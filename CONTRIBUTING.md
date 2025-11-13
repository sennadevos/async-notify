# Contributing to async-notify

Thank you for your interest in contributing to async-notify! This document provides guidelines and instructions for contributing.

## Code of Conduct

By participating in this project, you agree to maintain a respectful and inclusive environment for everyone.

## How to Contribute

### Reporting Bugs

Before creating bug reports, please check the existing issues to avoid duplicates. When creating a bug report, include:

- **Description**: Clear and concise description of the bug
- **Steps to Reproduce**: Detailed steps to reproduce the behavior
- **Expected Behavior**: What you expected to happen
- **Actual Behavior**: What actually happened
- **Environment**: OS, compiler version, ncurses version
- **Additional Context**: Screenshots, logs, or other relevant information

### Suggesting Enhancements

Enhancement suggestions are welcome! Please include:

- **Use Case**: Describe the problem you're trying to solve
- **Proposed Solution**: Your suggested approach
- **Alternatives**: Other solutions you've considered
- **Additional Context**: Examples, mockups, or references

### Pull Requests

1. **Fork the Repository**
   ```bash
   git clone https://github.com/sennadevos/async-notify.git
   cd async-notify
   ```

2. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make Your Changes**
   - Follow the existing code style
   - Add comments for complex logic
   - Update documentation as needed

4. **Test Your Changes**
   ```bash
   make clean
   make
   ./async echo "test"
   ```

5. **Commit Your Changes**
   ```bash
   git add .
   git commit -m "Add feature: description of your changes"
   ```
   
   Use clear commit messages following this format:
   - `Add feature: description`
   - `Fix bug: description`
   - `Update docs: description`
   - `Refactor: description`

6. **Push to Your Fork**
   ```bash
   git push origin feature/your-feature-name
   ```

7. **Open a Pull Request**
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Fill in the PR template with details

## Development Guidelines

### Code Style

- **Indentation**: Use 4 spaces (no tabs)
- **Braces**: Opening brace on same line
- **Naming**:
  - Functions: `camelCase`
  - Variables: `snake_case`
  - Classes/Structs: `PascalCase`
  - Constants: `UPPER_SNAKE_CASE`
- **Comments**: Use `//` for single-line, `/* */` for multi-line
- **Line Length**: Aim for 100 characters max

### Building and Testing

```bash
# Clean build
make clean && make

# Test basic functionality
./async sleep 2
./async echo "Hello World"
./async ls /nonexistent  # Test failure case

# Test with complex commands
./async "find . -name '*.cpp' | wc -l"
```

### Documentation

- Update README.md if adding new features
- Add inline comments for complex logic
- Update examples if changing behavior
- Keep documentation clear and concise

## Project Structure

```
async-notify/
├── async.cpp           # Main source code
├── Makefile           # Build configuration
├── README.md          # Project documentation
├── LICENSE            # MIT License
├── CONTRIBUTING.md    # This file
├── .github/
│   └── workflows/     # CI/CD workflows
└── docs/              # Additional documentation
```

## Feature Ideas

Looking for something to work on? Here are some ideas:

- [ ] Add command output capture and display in notification
- [ ] Support for multiple concurrent background commands
- [ ] Configuration file for notification appearance
- [ ] Desktop notification integration (libnotify)
- [ ] Command history and status tracking
- [ ] Progress indicators for long-running commands
- [ ] Support for command timeouts
- [ ] Add verbose/debug mode
- [ ] Create man page documentation
- [ ] Add shell completion scripts (bash, zsh, fish)

## Questions?

If you have questions, feel free to:

- Open an issue with the "question" label
- Check existing issues and discussions
- Contact the maintainers

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to async-notify! 🎉
