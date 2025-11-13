# async-notify

[![Build Status](https://github.com/sennadevos/async-notify/workflows/CI/badge.svg)](https://github.com/sennadevos/async-notify/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Release](https://img.shields.io/github/v/release/sennadevos/async-notify)](https://github.com/sennadevos/async-notify/releases)

> Asynchronous command execution with terminal notifications

**async-notify** is a lightweight C++ command-line tool that executes commands in background threads without blocking your terminal. When commands complete, it displays beautiful ncurses-based notifications with color-coded status indicators.

![Demo](docs/demo.gif)

## ✨ Features

- 🚀 **Non-blocking execution** - Run commands in the background and immediately regain control of your terminal
- 🎨 **Visual notifications** - Beautiful ncurses-based completion notifications with color-coded status
- 📊 **Exit code tracking** - See whether commands succeeded or failed with detailed exit codes
- 🧵 **Thread-based architecture** - Uses modern C++11 threading for robust background execution
- 🔌 **Process forking** - Fully detaches from the terminal for true background operation
- 💻 **Cross-platform** - Works on any Linux/Unix system with ncurses support

## 📋 Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Building from Source](#building-from-source)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

## 🚀 Installation

### From Release Binaries

Download the latest release from the [releases page](https://github.com/sennadevos/async-notify/releases):

```bash
# Download and install (replace VERSION with actual version)
wget https://github.com/sennadevos/async-notify/releases/download/vVERSION/async
chmod +x async
sudo mv async /usr/local/bin/
```

### Using Package Managers

#### Homebrew (macOS/Linux)

```bash
brew tap sennadevos/async-notify
brew install async-notify
```

#### From Source

See [Building from Source](#building-from-source) section below.

## 💡 Usage

### Basic Syntax

```bash
async <command> [args...]
```

### Examples

**Run a long-running download:**
```bash
async wget https://example.com/large-file.zip
```

**Execute a time-consuming build:**
```bash
async make -j4
```

**Run a sleep command for testing:**
```bash
async sleep 10
```

**Execute a script:**
```bash
async ./my-script.sh --option value
```

**Run commands with pipes and redirects:**
```bash
async "find / -name '*.log' > search-results.txt 2>&1"
```

### What Happens

1. The command starts executing in the background
2. Control returns to your terminal immediately
3. You can continue working normally
4. When the command completes, a notification appears
5. Press any key to dismiss the notification

## 🔧 How It Works

**async-notify** uses a sophisticated multi-threaded architecture:

1. **Command Parsing**: The tool parses your command and arguments
2. **Process Forking**: Forks a new process and creates a new session to detach from the terminal
3. **Background Execution**: Spawns a worker thread to execute the command
4. **Monitoring**: A separate monitoring thread waits for command completion
5. **Notification**: When complete, ncurses takes control of the terminal to display a notification
6. **User Acknowledgment**: Press any key to dismiss and return to normal operation

### Architecture

```
┌─────────────────────────────────────────────┐
│           Main Process (Parent)             │
│  • Parse arguments                          │
│  • Fork child process                       │
│  • Exit immediately (return to terminal)    │
└─────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────┐
│          Child Process (Daemon)             │
│  ┌───────────────────────────────────────┐  │
│  │      Worker Thread                    │  │
│  │  • Execute command via system()       │  │
│  │  • Capture exit status                │  │
│  │  • Signal completion                  │  │
│  └───────────────────────────────────────┘  │
│  ┌───────────────────────────────────────┐  │
│  │      Monitor Thread                   │  │
│  │  • Wait for completion signal         │  │
│  │  • Initialize ncurses                 │  │
│  │  • Display notification               │  │
│  │  • Wait for user dismissal            │  │
│  └───────────────────────────────────────┘  │
└─────────────────────────────────────────────┘
```

## 🛠️ Building from Source

### Prerequisites

- C++11 or later compiler (g++ or clang++)
- ncurses development library
- pthread support (usually included with g++)
- make

### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential libncurses5-dev libncursesw5-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install -y gcc-c++ ncurses-devel make
```

**Arch Linux:**
```bash
sudo pacman -S base-devel ncurses
```

**macOS:**
```bash
xcode-select --install
brew install ncurses
```

### Build

```bash
git clone https://github.com/sennadevos/async-notify.git
cd async-notify
make
```

### Install

```bash
sudo make install
```

This installs the binary to `/usr/local/bin/async`.

### Uninstall

```bash
sudo make uninstall
```

## 📚 Documentation

### Exit Code Handling

- **Normal termination**: Uses `WEXITSTATUS()` to extract the exit code
- **Signal termination**: Calculates exit code as 128 + signal number
- **Success**: Exit code 0 (displayed in green)
- **Failure**: Non-zero exit code (displayed in red)

### Terminal State Management

The tool properly manages terminal state:

- Saves terminal state before displaying notification
- Uses ncurses alternate screen buffer
- Restores original terminal state after dismissal
- Handles terminal resize gracefully

### Thread Synchronization

Uses modern C++ synchronization primitives:

- `std::mutex` for protecting shared state
- `std::condition_variable` for efficient waiting
- Lock guards for exception-safe locking

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR_USERNAME/async-notify.git`
3. Create a feature branch: `git checkout -b feature/amazing-feature`
4. Make your changes
5. Build and test: `make && ./async echo "test"`
6. Commit your changes: `git commit -m 'Add amazing feature'`
7. Push to the branch: `git push origin feature/amazing-feature`
8. Open a Pull Request

### Code Style

- Follow the existing code style
- Use meaningful variable and function names
- Add comments for complex logic
- Ensure code compiles without warnings

## 🐛 Troubleshooting

### "ncurses.h: No such file or directory"

Install the ncurses development library:
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### "undefined reference to pthread_create"

Make sure to link with pthread:
```bash
g++ async.cpp -o async -lncurses -lpthread
```

### Notification doesn't appear

- Ensure your terminal supports ncurses
- Check that the command actually completed
- Verify the child process is running: `ps aux | grep async`

### Terminal state corrupted

If the notification is interrupted, your terminal might be in a bad state. Reset it with:
```bash
reset
```

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Built with [ncurses](https://invisible-island.net/ncurses/) for terminal UI
- Inspired by Unix tools like `nohup` and `screen`
- Thanks to all contributors!

## 📬 Contact

- **Author**: Senna De Vos
- **GitHub**: [@sennadevos](https://github.com/sennadevos)
- **Issues**: [GitHub Issues](https://github.com/sennadevos/async-notify/issues)

---

<p align="center">Made with ❤️ by <a href="https://github.com/sennadevos">Senna De Vos</a></p>
