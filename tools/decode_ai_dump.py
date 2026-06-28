from __future__ import annotations

import re
import sys
from pathlib import Path


ROW_RE = re.compile(r"\[AI:DUMP:ROW\s+(\d{3})\]\s+([0-9A-Fa-f]+)")


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: decode_ai_dump.py <uart_log.txt> <out.pgm>")
        return 1

    log_path = Path(sys.argv[1])
    out_path = Path(sys.argv[2])
    rows: dict[int, bytes] = {}

    text = log_path.read_text(encoding="utf-8", errors="ignore")
    for m in ROW_RE.finditer(text):
        row_idx = int(m.group(1))
        hex_data = m.group(2).strip()
        if len(hex_data) != 224 * 2:
            continue
        rows[row_idx] = bytes.fromhex(hex_data)

    if len(rows) != 224:
        print(f"expected 224 rows, got {len(rows)}")
        return 2

    payload = b"".join(rows[i] for i in range(224))
    header = f"P5\n224 224\n255\n".encode("ascii")
    out_path.write_bytes(header + payload)
    print(f"wrote {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
