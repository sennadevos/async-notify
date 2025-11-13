# Security Policy

## Supported Versions

We release patches for security vulnerabilities in the following versions:

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |
| < 1.0   | :x:                |

## Reporting a Vulnerability

The async-notify team takes security bugs seriously. We appreciate your efforts to responsibly disclose your findings.

### How to Report

**Please do not report security vulnerabilities through public GitHub issues.**

Instead, please report security vulnerabilities by:

1. **Email**: Send an email to the repository maintainer through GitHub
2. **GitHub Security Advisory**: Use the [GitHub Security Advisory](https://github.com/sennadevos/async-notify/security/advisories/new) feature (preferred)

### What to Include

Please include the following information in your report:

- Type of issue (e.g., buffer overflow, command injection, etc.)
- Full paths of source file(s) related to the manifestation of the issue
- The location of the affected source code (tag/branch/commit or direct URL)
- Any special configuration required to reproduce the issue
- Step-by-step instructions to reproduce the issue
- Proof-of-concept or exploit code (if possible)
- Impact of the issue, including how an attacker might exploit it

### Response Timeline

- **Initial Response**: Within 48 hours
- **Status Update**: Within 7 days
- **Fix Timeline**: Depends on severity
  - Critical: Within 7 days
  - High: Within 30 days
  - Medium: Within 90 days
  - Low: Next release cycle

### Disclosure Policy

When we receive a security bug report, we will:

1. Confirm the problem and determine affected versions
2. Audit code to find any similar problems
3. Prepare fixes for all supported versions
4. Release patches as soon as possible

We ask that you:

- Give us reasonable time to fix the issue before public disclosure
- Make a good faith effort to avoid privacy violations, data destruction, and service interruption
- Do not access or modify data that doesn't belong to you
- Contact us before sharing information about the vulnerability with others

### Security Best Practices

When using async-notify:

**Command Injection Prevention**

The tool executes commands through the system shell. Be aware that:

- Commands are executed with the user's privileges
- Shell metacharacters are interpreted (pipes, redirects, etc.)
- User input should be validated before passing to async
- Avoid constructing commands from untrusted input

**Example of unsafe usage:**
```bash
# DON'T DO THIS - vulnerable to command injection
user_input="file.txt; rm -rf /"
async cat "$user_input"
```

**Example of safer usage:**
```bash
# Validate and sanitize input first
if [[ "$user_input" =~ ^[a-zA-Z0-9._-]+$ ]]; then
    async cat "$user_input"
else
    echo "Invalid filename"
fi
```

**Process Isolation**

- Background processes run with the same user privileges
- No privilege escalation is performed
- Processes are isolated in separate sessions
- No network exposure by default

**Terminal Security**

- ncurses notifications appear on the user's terminal
- No persistent storage of commands or output
- Terminal state is properly restored
- No logging of sensitive information

### Known Limitations

**Not Security Issues**

The following are known limitations and not considered security vulnerabilities:

1. **Command execution with user privileges**: By design, commands run with the user's permissions
2. **Shell interpretation**: The tool uses `system()` which invokes the shell - this is intentional
3. **No sandboxing**: Commands are not sandboxed or containerized
4. **No output sanitization**: Command output is not filtered or sanitized

### Security Updates

Security updates will be:

- Released as patch versions (e.g., 1.0.1, 1.0.2)
- Documented in CHANGELOG.md
- Announced in GitHub releases
- Tagged with the `security` label

### Attribution

We appreciate responsible disclosure and will acknowledge security researchers who report valid vulnerabilities (unless they prefer to remain anonymous).

### Questions

If you have questions about this security policy, please open a GitHub issue with the `question` label.

---

**Thank you for helping keep async-notify and its users safe!**
