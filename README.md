# zord

A terminal text editor written in C++20.

## Architecture

zord is built around a few core components:

- **PieceTable** — the text buffer, implemented as a piece table for efficient insert/delete without copying the full document on every edit.
- **RenderEngine** — handles terminal rendering: screen clears, cursor positioning, and cursor style (block, underline, bar — blinking or steady).
- **KeySystem** — CRTP-based abstraction for key handling. `ZordKeySystem` is the default implementation.
- **Editor** — top-level class that owns the buffer, raw mode, key system, and render engine. Templated on the key system: `Editor<ZordKeySystem>`.
- **RawMode** — puts the terminal into raw mode on construction and restores it on destruction (or via `atexit`).

## Building

Requires clang++ with C++20 support.

```bash
./build.sh
```

This compiles all sources and produces the `zord` executable.

## Running

```bash
./zord
```

## Project Structure

```
include/          # Header files
  editor.hpp
  piece-table-buffer.hpp
  text-buffer.hpp
  render-engine.hpp
  key-system.hpp
  zord-key-system.hpp
  raw_mode.hpp
  position.hpp
src/
  main.cpp
  buffers/        piece-table-buffer.cpp
  editor/         editor.cpp
  key-system/     zord-key-system.cpp
  render-engine/  render-engine.cpp
  terminal/       raw_mode.cpp
build.sh
```
