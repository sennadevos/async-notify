# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial release of async-notify
- Asynchronous command execution with process forking
- ncurses-based terminal notifications
- Color-coded status indicators (green for success, red for failure)
- Exit code tracking and display
- Multi-threaded architecture with worker and monitor threads
- Comprehensive documentation and examples

### Features
- Non-blocking terminal execution
- Background command processing
- Visual completion notifications
- Support for complex commands with pipes and redirects
- Automatic terminal state management

## [1.0.0] - 2025-11-13

### Added
- First stable release
- Core functionality: background command execution
- ncurses notification system
- Thread-based architecture
- MIT License
- Comprehensive README with examples
- Build system with Makefile
- GitHub Actions CI/CD pipeline

[Unreleased]: https://github.com/sennadevos/async-notify/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/sennadevos/async-notify/releases/tag/v1.0.0
