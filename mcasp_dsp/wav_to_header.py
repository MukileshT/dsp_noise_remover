"""Convert a PCM WAV file into a self-contained C header."""

from __future__ import annotations

import argparse
import re
import wave
from pathlib import Path


def identifier(name: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_]", "_", name)
    value = re.sub(r"_+", "_", value).strip("_") or "audio_samples"
    if value[0].isdigit():
        value = "audio_" + value
    return value.lower()


def convert(input_path: Path, output_path: Path, symbol: str, values_per_line: int) -> None:
    with wave.open(str(input_path), "rb") as wav:
        channels = wav.getnchannels()
        sample_width = wav.getsampwidth()
        sample_rate = wav.getframerate()
        frame_count = wav.getnframes()
        compression = wav.getcomptype()
        raw = wav.readframes(frame_count)

    if compression != "NONE":
        raise ValueError("only uncompressed PCM WAV files are supported")
    if sample_width not in (1, 2):
        raise ValueError("only 8-bit and 16-bit PCM WAV files are supported")

    if sample_width == 1:
        values = [sample - 128 for sample in raw]
        c_type = "int8_t"
    else:
        values = [int.from_bytes(raw[i:i + 2], "little", signed=True)
                  for i in range(0, len(raw), 2)]
        c_type = "int16_t"

    guard = identifier(output_path.stem).upper() + "_H"
    prefix = guard[:-2] + "_"
    lines = [
        f"#ifndef {guard}",
        f"#define {guard}",
        "",
        "#include <stdint.h>",
        "",
        f"#define {prefix}SAMPLE_RATE {sample_rate}u",
        f"#define {prefix}CHANNELS {channels}u",
        f"#define {prefix}SAMPLE_COUNT {len(values)}u",
        "",
        f"static const {c_type} {symbol}[{len(values)}] = {{",
    ]
    for start in range(0, len(values), values_per_line):
        chunk = values[start:start + values_per_line]
        lines.append("    " + ", ".join(str(value) for value in chunk) + ",")
    lines.extend(["};", "", "#endif", ""])
    output_path.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="input PCM WAV file")
    parser.add_argument("output", type=Path, nargs="?", help="output .h file")
    parser.add_argument("--symbol", help="C array name; defaults to the input filename")
    parser.add_argument("--per-line", type=int, default=12, help="samples per C source line")
    args = parser.parse_args()

    if args.per_line < 1:
        parser.error("--per-line must be positive")
    output = args.output or args.input.with_suffix(".h")
    symbol = identifier(args.symbol or args.input.stem)
    convert(args.input, output, symbol, args.per_line)
    print(f"Wrote {output} ({symbol})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
