# Image watcher

A minimal Linux utility built using raw **inotify syscalls** to automatically detect and move newly added image files from one directory to another.

# Blog
- [inotify - watching file changes efficiently](https://techrail.in/authors/ankit/2026/inotify-linux)

## Features

- Watches a directory in real-time (no polling)
- Detects newly added files using inotify
- Filters image files (`.jpg`, `.jpeg`, `.webp`, `.png`)
- Moves files instantly using `rename()`

## How It Works

- Uses Linux **inotify** to listen for:
  - `IN_CLOSE_WRITE` → file finished writing
  - `IN_MOVED_TO` → file moved into directory
- Parses kernel event buffer
- Moves valid image files to destination folder

## Setup

```bash
gcc watcher.c -o watcher
./watcher
