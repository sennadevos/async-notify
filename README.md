# async-notify

Asynchronous command execution with terminal notifications.

## Overview

**async-notify** is a lightweight C++ tool that runs commands in the background without blocking your terminal. When commands complete, it displays a notification using ncurses.

## Features

- Non-blocking execution - regain terminal control immediately
- Visual ncurses notifications with color-coded status
- Exit code tracking
- Multi-threaded architecture

## Installation

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libncurses5-dev

# Fedora/RHEL
sudo dnf install gcc-c++ ncurses-devel

# Arch Linux
sudo pacman -S base-devel ncurses
```

### Build and Install

```bash
git clone https://github.com/sennadevos/async-notify.git
cd async-notify
make
sudo make install
```

## Usage

```bash
async <command> [args...]
```

### Examples

```bash
# Long-running download
async wget https://example.com/large-file.zip

# Time-consuming build
async make -j4

# Any command
async sleep 10
```

The command runs in the background, and a notification appears when it completes showing the exit status.

## How It Works

The tool uses process forking and threading:

1. Forks a background process and returns control immediately
2. Worker thread executes the command
3. Monitor thread waits for completion
4. ncurses notification displays the result
5. Press any key to dismiss

## Building

```bash
make              # Build
make clean        # Clean build artifacts
make install      # Install to /usr/local/bin
make uninstall    # Remove from /usr/local/bin
```

## Requirements

- C++11 or later
- ncurses library
- pthread support
- Linux/Unix system

## License

MIT License - see LICENSE file for details.

## Author

Senna De Vos ([@sennadevos](https://github.com/sennadevos))
