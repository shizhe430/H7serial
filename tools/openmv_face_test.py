"""Isolated OpenMV-style face test for the H743 JPEG diagnostic stream.

The board only supplies camera-1 JPEG frames in OPENMV_HOST_DIAG mode. This
tool performs grayscale Haar detection and an LBP histogram match on the PC.
It intentionally has no connection to the production YuNet/SFace database.
"""

from __future__ import annotations

import argparse
import json
import queue
import threading
import time
from pathlib import Path
import tkinter as tk
from tkinter import messagebox, ttk

import cv2
import numpy as np
import serial


MAGIC = b"FAV1"
TAIL = b"FAE1"
HEADER_SIZE = 48
DEFAULT_CASCADE = Path(cv2.data.haarcascades) / "haarcascade_frontalface_default.xml"
DEFAULT_DB = Path(__file__).resolve().parents[1] / "tmp" / "openmv_face_id1.npz"
DEFAULT_LOG = Path(__file__).resolve().parents[1] / "tmp" / "openmv_face_results.jsonl"


def uniform_lbp_bins() -> np.ndarray:
    mapping = np.full(256, 58, dtype=np.uint8)
    index = 0
    for value in range(256):
        bits = ((value >> np.arange(8)) & 1).astype(np.uint8)
        transitions = int(np.count_nonzero(bits != np.roll(bits, 1)))
        if transitions <= 2:
            mapping[value] = index
            index += 1
    return mapping


LBP_BINS = uniform_lbp_bins()


def lbp_descriptor(face_bgr: np.ndarray) -> np.ndarray:
    gray = cv2.cvtColor(face_bgr, cv2.COLOR_BGR2GRAY)
    gray = cv2.resize(gray, (128, 128), interpolation=cv2.INTER_AREA)
    gray = cv2.equalizeHist(gray)
    center = gray[1:-1, 1:-1]
    code = np.zeros_like(center, dtype=np.uint8)
    neighbors = (
        gray[:-2, :-2], gray[:-2, 1:-1], gray[:-2, 2:], gray[1:-1, 2:],
        gray[2:, 2:], gray[2:, 1:-1], gray[2:, :-2], gray[1:-1, :-2],
    )
    for bit, sample in enumerate(neighbors):
        code |= ((sample >= center).astype(np.uint8) << bit)
    mapped = LBP_BINS[code]

    histograms = []
    for row in np.array_split(mapped, 8, axis=0):
        for cell in np.array_split(row, 8, axis=1):
            hist = np.bincount(cell.reshape(-1), minlength=59).astype(np.float32)
            hist /= max(float(hist.sum()), 1.0)
            histograms.append(hist)
    return np.concatenate(histograms).astype(np.float32)


def chi_square_distance(left: np.ndarray, right: np.ndarray) -> float:
    denominator = left + right + 1e-8
    return float(0.5 * np.sum(((left - right) ** 2) / denominator) / 64.0)


class Fav1Parser:
    def __init__(self) -> None:
        self.buffer = bytearray()

    def feed(self, data: bytes) -> list[bytes]:
        self.buffer.extend(data)
        frames = []
        while True:
            start = self.buffer.find(MAGIC)
            if start < 0:
                del self.buffer[:-3]
                break
            if start:
                del self.buffer[:start]
            if len(self.buffer) < HEADER_SIZE:
                break
            jpeg_len = int.from_bytes(self.buffer[12:16], "little")
            if not 4 <= jpeg_len <= 65535:
                del self.buffer[0]
                continue
            packet_len = HEADER_SIZE + jpeg_len + len(TAIL)
            if len(self.buffer) < packet_len:
                break
            packet = bytes(self.buffer[:packet_len])
            if packet[-4:] != TAIL:
                del self.buffer[0]
                continue
            jpeg = packet[HEADER_SIZE:HEADER_SIZE + jpeg_len]
            if not jpeg.startswith(b"\xff\xd8") or not jpeg.endswith(b"\xff\xd9"):
                del self.buffer[0]
                continue
            frames.append(packet)
            del self.buffer[:packet_len]
        return frames


def decode_packet(packet: bytes) -> tuple[np.ndarray, dict]:
    header = packet[:HEADER_SIZE]
    jpeg_len = int.from_bytes(header[12:16], "little")
    image = cv2.imdecode(np.frombuffer(packet[HEADER_SIZE:HEADER_SIZE + jpeg_len], dtype=np.uint8), cv2.IMREAD_COLOR)
    if image is None:
        raise ValueError("invalid JPEG")
    flags = header[6]
    fields = {
        "camera": header[5],
        "status": header[7],
        "frame_id": int.from_bytes(header[8:12], "little"),
        "board_flags": flags,
        "board_detection": bool(flags & 1),
        "board_embedding": bool(flags & 2),
    }
    return image, fields


class OpenMVFaceTest:
    def __init__(self, port: str, baud: int, cascade_path: Path, db_path: Path,
                 log_path: Path = DEFAULT_LOG, auto_open: bool = False) -> None:
        self.port_name = port
        self.baud = baud
        self.cascade_path = cascade_path
        self.db_path = db_path
        self.log_path = log_path
        self.serial: serial.Serial | None = None
        self.stop_event = threading.Event()
        self.packet_queue: queue.Queue[bytes] = queue.Queue(maxsize=2)
        self.parser = Fav1Parser()
        self.reference: np.ndarray | None = None
        self.last_face: tuple[int, int, int, int] | None = None
        self.enroll_remaining = 0
        self.enroll_samples: list[np.ndarray] = []
        self.last_result = "WAITING FOR FAV1"
        self.match_count = 0
        self.no_match_count = 0
        self.best_distance: float | None = None
        self.frame_count = 0
        self.last_frame_time: float | None = None
        self.display_fps = 0.0
        self.cascade = cv2.CascadeClassifier(str(cascade_path))
        if self.cascade.empty():
            raise RuntimeError(f"cannot load Haar cascade: {cascade_path}")
        self.root = tk.Tk()
        self.root.title("OpenMV Face Test - isolated Haar + LBP")
        self.root.geometry("1100x760")
        self.image_label = tk.Label(self.root, bg="black")
        self.image_label.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=8, pady=8)
        self.panel = ttk.Frame(self.root, padding=10)
        self.panel.pack(side=tk.RIGHT, fill=tk.Y)
        self.status = tk.StringVar(value="CLOSED")
        self.result = tk.StringVar(value=self.last_result)
        self.metrics = tk.StringVar(value="No frame")
        self._build_controls()
        self.root.protocol("WM_DELETE_WINDOW", self.close)
        self.root.after(30, self.process_queue)
        self.load_reference()
        if auto_open:
            self.root.after(100, self.open_serial)

    def _build_controls(self) -> None:
        ttk.Label(self.panel, text="OpenMV-style isolated test", font=("Segoe UI", 14, "bold")).pack(anchor="w", pady=(0, 12))
        ttk.Label(self.panel, textvariable=self.status).pack(anchor="w", pady=2)
        ttk.Button(self.panel, text="Open serial", command=self.open_serial).pack(fill=tk.X, pady=4)
        ttk.Button(self.panel, text="Close serial", command=self.close_serial).pack(fill=tk.X, pady=4)
        ttk.Separator(self.panel).pack(fill=tk.X, pady=10)
        ttk.Button(self.panel, text="Enroll ID=1", command=self.enroll).pack(fill=tk.X, pady=4)
        ttk.Button(self.panel, text="Clear ID=1", command=self.clear_reference).pack(fill=tk.X, pady=4)
        ttk.Label(self.panel, text="Match threshold (chi-square)").pack(anchor="w", pady=(12, 2))
        self.threshold = tk.DoubleVar(value=0.55)
        ttk.Entry(self.panel, textvariable=self.threshold, width=12).pack(anchor="w")
        ttk.Label(self.panel, textvariable=self.metrics, justify=tk.LEFT).pack(anchor="w", pady=16)
        ttk.Label(self.panel, textvariable=self.result, foreground="green", justify=tk.LEFT, font=("Segoe UI", 12, "bold")).pack(anchor="w", pady=12)
        ttk.Label(self.panel, text=f"DB: {self.db_path}", wraplength=270, justify=tk.LEFT).pack(anchor="w", pady=8)

    def load_reference(self) -> None:
        if self.db_path.exists():
            try:
                self.reference = np.load(self.db_path)["descriptor"].astype(np.float32)
                self.result.set("Reference: ID=1 READY\nWAITING FOR FACE")
            except Exception as exc:
                self.result.set(f"DB LOAD ERROR\n{exc}")

    def open_serial(self) -> None:
        if self.serial is not None and self.serial.is_open:
            return
        try:
            self.serial = serial.Serial(self.port_name, self.baud, timeout=0.1)
            self.stop_event.clear()
            threading.Thread(target=self.reader, daemon=True).start()
            self.status.set(f"OPEN {self.port_name} @ {self.baud}")
        except Exception as exc:
            self.status.set("OPEN FAILED")
            messagebox.showerror("Serial", str(exc))

    def close_serial(self) -> None:
        self.stop_event.set()
        if self.serial is not None:
            try:
                self.serial.close()
            except serial.SerialException:
                pass
            self.serial = None
        self.status.set("CLOSED")

    def reader(self) -> None:
        while not self.stop_event.is_set():
            current = self.serial
            if current is None or not current.is_open:
                break
            try:
                data = current.read(4096)
            except serial.SerialException:
                break
            for packet in self.parser.feed(data):
                try:
                    self.packet_queue.put_nowait(packet)
                except queue.Full:
                    try:
                        self.packet_queue.get_nowait()
                        self.packet_queue.put_nowait(packet)
                    except queue.Empty:
                        pass

    def detect(self, image: np.ndarray) -> tuple[tuple[int, int, int, int] | None, float, float]:
        start = time.perf_counter()
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray = cv2.equalizeHist(gray)
        faces = self.cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        if len(faces) == 0:
            return None, 0.0, (time.perf_counter() - start) * 1000.0
        face = max(faces, key=lambda item: int(item[2]) * int(item[3]))
        x, y, w, h = map(int, face)
        return (x, y, w, h), 1.0, (time.perf_counter() - start) * 1000.0

    def handle_packet(self, packet: bytes) -> None:
        image, fields = decode_packet(packet)
        face, score, detect_ms = self.detect(image)
        identity_ms = 0.0
        distance = None
        match = False
        enrolled_now = False
        if face is not None:
            x, y, w, h = face
            margin_x, margin_y = int(w * 0.15), int(h * 0.15)
            crop = image[max(0, y - margin_y):min(image.shape[0], y + h + margin_y), max(0, x - margin_x):min(image.shape[1], x + w + margin_x)]
            start = time.perf_counter()
            descriptor = lbp_descriptor(crop)
            identity_ms = (time.perf_counter() - start) * 1000.0
            if self.enroll_remaining > 0:
                self.enroll_samples.append(descriptor)
                self.enroll_remaining -= 1
                if self.enroll_remaining == 0:
                    self.reference = np.mean(self.enroll_samples, axis=0).astype(np.float32)
                    self.db_path.parent.mkdir(parents=True, exist_ok=True)
                    np.savez_compressed(self.db_path, descriptor=self.reference, id=np.array([1], dtype=np.uint16))
                    enrolled_now = True
                else:
                    self.result.set(f"ENROLL ID=1\n{self.enroll_remaining} FRAMES LEFT")
            elif self.reference is not None:
                distance = chi_square_distance(self.reference, descriptor)
                match = distance <= float(self.threshold.get())
                self.best_distance = distance if self.best_distance is None else min(self.best_distance, distance)
                if match:
                    self.match_count += 1
                else:
                    self.no_match_count += 1
            self.last_face = face
        else:
            self.last_face = None
        self.frame_count += 1
        now = time.perf_counter()
        if self.last_frame_time is not None:
            delta = now - self.last_frame_time
            if delta > 0:
                self.display_fps = 1.0 / delta
        self.last_frame_time = now
        total_ms = detect_ms + identity_ms
        if enrolled_now:
            result = "Reference: ID=1 READY\nENROLL COMPLETE"
        elif self.enroll_remaining > 0:
            result = f"ENROLL ID=1\n{self.enroll_remaining} FRAMES LEFT"
        elif face is None:
            result = "NO FACE"
        elif self.reference is None:
            result = "FACE DETECTED\nREFERENCE EMPTY"
        elif match:
            result = "Reference: ID=1 READY\nMATCH ID=1"
        else:
            result = "Reference: ID=1 READY\nNO MATCH"
        self.last_result = result
        self.result.set(result)
        distance_text = "-" if distance is None else f"{distance:.3f}"
        self.metrics.set(
            f"frame={fields['frame_id']} camera={fields['camera']}\n"
            f"haar={detect_ms:.1f} ms  lbp={identity_ms:.1f} ms\n"
            f"total={total_ms:.1f} ms  display={self.display_fps:.2f} fps\n"
            f"distance={distance_text} threshold={float(self.threshold.get()):.3f}\n"
            f"match={self.match_count} no_match={self.no_match_count}\n"
            f"board_flags=0x{fields['board_flags']:02x} (expected 0x00)"
        )
        record = {
            "time": time.time(), "frame": fields["frame_id"], "camera": fields["camera"],
            "face": face is not None, "detect_ms": round(detect_ms, 3),
            "lbp_ms": round(identity_ms, 3), "distance": distance, "match": match,
            "reference_ready": self.reference is not None, "result": result.replace("\n", " "),
        }
        with self.log_path.open("a", encoding="ascii") as log_file:
            log_file.write(json.dumps(record, separators=(",", ":")) + "\n")
        display = image.copy()
        if face is not None:
            x, y, w, h = face
            color = (0, 220, 0) if match or self.reference is None else (0, 165, 255)
            cv2.rectangle(display, (x, y), (x + w, y + h), color, 2)
            cv2.putText(display, "ID=1 MATCH" if match else "FACE", (x, max(18, y - 6)), cv2.FONT_HERSHEY_SIMPLEX, 0.55, color, 2, cv2.LINE_AA)
        cv2.putText(display, result.replace("\n", " / "), (8, 22), cv2.FONT_HERSHEY_SIMPLEX, 0.55, (0, 255, 0) if match else (0, 200, 255), 2, cv2.LINE_AA)
        display = cv2.cvtColor(display, cv2.COLOR_BGR2RGB)
        image = cv2.resize(display, (640, int(display.shape[0] * 640 / display.shape[1])))
        photo = tk.PhotoImage(data=cv2.imencode(".ppm", image)[1].tobytes(), format="PPM")
        self.image_label.configure(image=photo)
        self.image_label.image = photo

    def process_queue(self) -> None:
        try:
            packet = self.packet_queue.get_nowait()
            self.handle_packet(packet)
        except queue.Empty:
            pass
        except Exception as exc:
            self.status.set(f"FRAME ERROR: {exc}")
        self.root.after(30, self.process_queue)

    def enroll(self) -> None:
        if self.last_face is None:
            messagebox.showinfo("Enroll", "Keep one face visible, then click Enroll again.")
            return
        self.enroll_samples = []
        self.enroll_remaining = 5
        self.result.set("ENROLL ID=1\n5 FRAMES LEFT")

    def clear_reference(self) -> None:
        self.reference = None
        self.enroll_remaining = 0
        self.enroll_samples = []
        self.db_path.unlink(missing_ok=True)
        self.result.set("REFERENCE EMPTY")
        self.match_count = 0
        self.no_match_count = 0
        self.best_distance = None

    def close(self) -> None:
        self.close_serial()
        self.root.destroy()


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="COM28")
    parser.add_argument("--baud", type=int, default=921600)
    parser.add_argument("--cascade", type=Path, default=DEFAULT_CASCADE)
    parser.add_argument("--db", type=Path, default=DEFAULT_DB)
    parser.add_argument("--log", type=Path, default=DEFAULT_LOG)
    parser.add_argument("--auto-open", action="store_true")
    args = parser.parse_args()
    args.db.parent.mkdir(parents=True, exist_ok=True)
    args.log.parent.mkdir(parents=True, exist_ok=True)
    app = OpenMVFaceTest(args.port, args.baud, args.cascade, args.db, args.log, auto_open=args.auto_open)
    app.root.mainloop()


if __name__ == "__main__":
    main()
