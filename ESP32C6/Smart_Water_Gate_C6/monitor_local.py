import argparse
import sys
import time

import serial


def main() -> int:
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")

    parser = argparse.ArgumentParser(description="Capture ESP32-C6 serial output")
    parser.add_argument("--port", default="COM29")
    parser.add_argument("--seconds", type=int, default=60)
    parser.add_argument("--output", default="boot_log_local.txt")
    args = parser.parse_args()

    chunks: list[str] = []
    with serial.Serial(args.port, 115200, timeout=0.2) as device:
        device.dtr = False
        device.rts = True
        time.sleep(0.1)
        device.dtr = True
        time.sleep(0.1)
        device.rts = False
        device.dtr = False
        time.sleep(1)

        deadline = time.monotonic() + args.seconds
        while time.monotonic() < deadline:
            data = device.read(device.in_waiting or 1)
            if not data:
                continue
            text = data.decode("utf-8", errors="replace")
            chunks.append(text)
            sys.stdout.write(text)
            sys.stdout.flush()

    with open(args.output, "w", encoding="utf-8") as log:
        log.write("".join(chunks))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
