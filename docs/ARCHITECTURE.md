# Architecture Documentation

This document provides detailed information about the internal architecture and design decisions of async-notify.

## Overview

**async-notify** is designed as a multi-process, multi-threaded application that leverages Unix process forking and C++11 threading to achieve non-blocking command execution with terminal notifications.

## Component Architecture

### Process Model

The application uses a two-process model:

**Parent Process (Main)**

The parent process is responsible for initial setup and immediately returns control to the terminal. Its lifecycle is very short:

1. Parse command-line arguments
2. Validate input
3. Fork a child process
4. Exit immediately (returns exit code 0)

This design ensures the user's terminal is never blocked and can be used immediately after invoking the command.

**Child Process (Daemon)**

The child process becomes a daemon by calling `setsid()`, which creates a new session and detaches from the controlling terminal. This process contains the actual execution logic:

1. Create a new session (become session leader)
2. Spawn worker thread for command execution
3. Spawn monitor thread for notification
4. Wait for both threads to complete
5. Exit when notification is dismissed

### Thread Model

The child process uses two threads:

**Worker Thread**

Responsible for executing the user's command:

- Executes command using `system()` call
- Captures exit status using `WIFEXITED()` and `WEXITSTATUS()` macros
- Handles signal termination (calculates exit code as 128 + signal number)
- Updates shared state with completion status
- Notifies monitor thread via condition variable

**Monitor Thread**

Responsible for displaying the notification:

- Waits on condition variable for command completion
- Initializes ncurses terminal UI
- Creates centered notification window
- Displays command, exit code, and status
- Waits for user acknowledgment (any key press)
- Cleans up ncurses and restores terminal state

### Synchronization

Thread synchronization is achieved using C++11 primitives:

**Shared State Structure**

```cpp
struct CommandState {
    bool completed = false;
    int exit_code = -1;
    std::string command;
    std::mutex mtx;
    std::condition_variable cv;
};
```

**Synchronization Flow**

1. Worker thread locks mutex, updates state, unlocks, and notifies
2. Monitor thread waits on condition variable with predicate
3. When notified, monitor thread checks completion flag
4. If true, proceeds to display notification

This design ensures:
- No race conditions on shared state
- Efficient waiting (no busy-waiting)
- Exception-safe locking (using lock guards)

## Terminal Management

### ncurses Integration

The notification system uses ncurses for terminal control:

**Initialization**
- `initscr()` - Initialize ncurses mode
- `cbreak()` - Disable line buffering
- `noecho()` - Don't echo input
- `curs_set(0)` - Hide cursor

**Color Support**
- Detect color capability with `has_colors()`
- Initialize color pairs for success (green) and failure (red)
- Apply colors using `wattron()` and `wattroff()`

**Window Management**
- Calculate centered position based on terminal dimensions
- Create bordered window with `newwin()`
- Handle terminal resize gracefully
- Use alternate screen buffer (automatic with ncurses)

**Cleanup**
- Delete window with `delwin()`
- Restore terminal state with `endwin()`

### Terminal State Preservation

ncurses automatically manages terminal state:
- Saves original terminal settings on `initscr()`
- Uses alternate screen buffer (doesn't disturb existing content)
- Restores original settings on `endwin()`

## Command Execution

### System Call Approach

The tool uses `system()` for command execution:

**Advantages:**
- Simple and reliable
- Handles shell metacharacters (pipes, redirects, etc.)
- Uses user's default shell
- Proper signal handling

**Exit Code Handling:**
```cpp
int status = system(command.c_str());
int exit_code = -1;

if (WIFEXITED(status)) {
    // Normal termination
    exit_code = WEXITSTATUS(status);
} else if (WIFSIGNALED(status)) {
    // Terminated by signal
    exit_code = 128 + WTERMSIG(status);
}
```

### Alternative Approaches Considered

**`fork()` + `exec()`**
- More control but requires manual argument parsing
- Doesn't handle shell features (pipes, redirects)
- More complex error handling

**`popen()`**
- Good for capturing output
- Not needed since we don't display output
- Similar complexity to `system()`

## Design Decisions

### Why Fork Instead of Just Threading?

Forking ensures complete detachment from the parent process:
- Parent can exit immediately
- Child becomes independent daemon
- No dependency on parent process lifetime
- Clean separation of concerns

### Why Two Threads in Child Process?

Separation of concerns:
- Worker thread focuses on command execution
- Monitor thread focuses on UI/notification
- Clean synchronization point (command completion)
- Easier to extend (e.g., add progress monitoring)

### Why ncurses Instead of Desktop Notifications?

Terminal-native approach:
- Works in SSH sessions
- No desktop environment required
- Consistent across all Unix systems
- User is already in terminal context
- Immediate visibility

### Why Not Capture Command Output?

Simplicity and focus:
- Tool focuses on notification, not output management
- Users can redirect output themselves if needed
- Avoids memory issues with large outputs
- Keeps notification UI simple

## Performance Considerations

### Memory Usage

Minimal memory footprint:
- Single command string stored
- Small shared state structure
- ncurses uses minimal memory
- No output buffering

### CPU Usage

Efficient waiting mechanisms:
- Condition variable (no busy-waiting)
- ncurses event-driven input
- Minimal overhead when idle

### Scalability

Current limitations:
- One notification per invocation
- No tracking of multiple concurrent commands
- No persistent state

Future improvements could add:
- Command queue management
- Multiple notification support
- Status dashboard

## Error Handling

### Process Forking Errors

If `fork()` fails:
- Print error message
- Return exit code 1
- No background execution

### Command Execution Errors

Command errors are captured:
- Non-zero exit codes displayed in notification
- Signal termination handled properly
- Red color indicates failure

### Terminal Errors

ncurses initialization is not explicitly error-checked:
- Assumes terminal supports ncurses
- Falls back gracefully if colors unavailable
- Window sizing adapts to terminal dimensions

## Security Considerations

### Command Injection

The tool passes commands directly to `system()`:
- Uses user's shell for execution
- No additional sanitization
- User is responsible for command safety
- Same security model as shell

### Process Isolation

Child process is isolated:
- New session (not part of parent's process group)
- Detached from controlling terminal
- Independent lifecycle

## Future Architecture Enhancements

### Potential Improvements

**Output Capture**
- Add option to capture and display command output
- Store output in temporary file
- Show in notification or separate viewer

**Multiple Commands**
- Track multiple concurrent background commands
- Show list of running/completed commands
- Allow cancellation of running commands

**Configuration**
- Support config file for notification appearance
- Customize colors, window size, timeout
- Per-command configuration

**Desktop Integration**
- Optional libnotify support for desktop notifications
- System tray integration
- Sound notifications

**Progress Monitoring**
- Detect long-running commands
- Show progress indicator
- Estimate time remaining

## Testing Strategy

### Manual Testing

Current testing approach:
- Test with various command types
- Verify success/failure notifications
- Check terminal state restoration
- Test with different terminal sizes

### Automated Testing

Future testing improvements:
- Unit tests for synchronization logic
- Integration tests for command execution
- Terminal UI tests with expect/tmux
- CI/CD pipeline tests

## Build System

### Makefile Design

Simple, portable Makefile:
- Standard targets (all, clean, install, uninstall)
- Configurable PREFIX for installation path
- Support for DESTDIR (package building)
- Minimal dependencies

### Compiler Requirements

C++11 minimum:
- `std::thread` support
- `std::mutex` and `std::condition_variable`
- Modern C++ features
- Cross-platform compatibility

## Conclusion

The architecture of async-notify prioritizes simplicity, reliability, and user experience. The multi-process, multi-threaded design ensures non-blocking operation while maintaining clean separation of concerns. The ncurses-based notification system provides immediate, terminal-native feedback without requiring external dependencies or desktop environments.
