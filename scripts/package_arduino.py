#!/usr/bin/env python3

from pathlib import Path
from shutil import copy2, copytree, rmtree

ROOT = Path(__file__).resolve().parent.parent
DEST = ROOT / "dist" / "AW9523B"
SRC = DEST / "src"

# Remove only the previously generated Arduino package.
if DEST.exists():
    rmtree(DEST)

(SRC / "aw9523b").mkdir(parents=True)

# Public Core headers.
copytree(
    ROOT / "core" / "include" / "aw9523b",
    SRC / "aw9523b",
    dirs_exist_ok=True
)

# Arduino adapter headers.
copytree(
    ROOT / "adapters" / "arduino" / "include" / "aw9523b",
    SRC / "aw9523b",
    dirs_exist_ok=True
)

# Core implementation.
for name in ("aw9523b.cpp", "result.cpp", "detail.hpp"):
    copy2(ROOT / "core" / "src" / name, SRC / name)

# Arduino adapter implementation.
for name in ("arduino_i2c.cpp", "arduino_platform.cpp"):
    copy2(ROOT / "adapters" / "arduino" / "src" / name, SRC / name)

# Distribution metadata and documentation.
copy2(
    ROOT / "packaging" / "arduino" / "library.properties",
    DEST / "library.properties"
)

for name in ("README.md", "LICENSE"):
    copy2(ROOT / name, DEST / name)

# Arduino example.
example = ROOT / "examples" / "arduino" / "basic" / "basic.ino"

if example.is_file():
    example_dest = DEST / "examples" / "Basic"
    example_dest.mkdir(parents=True)
    copy2(example, example_dest / "Basic.ino")

print(f"Arduino package generated: {DEST}")
