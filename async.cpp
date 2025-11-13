#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <chrono>

// Shared state between threads
struct CommandState {
    bool completed = false;
    int exit_code = -1;
    std::string command;
    std::mutex mtx;
    std::condition_variable cv;
};

// Execute command in background and capture exit status
void executeCommand(CommandState* state) {
    // Execute the command
    int status = system(state->command.c_str());
    
    // Get exit code
    int exit_code = -1;
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        exit_code = 128 + WTERMSIG(status);
    }
    
    // Update shared state
    {
        std::lock_guard<std::mutex> lock(state->mtx);
        state->exit_code = exit_code;
        state->completed = true;
    }
    state->cv.notify_one();
}

// Monitor completion and display notification
void monitorAndNotify(CommandState* state) {
    // Wait for command completion
    {
        std::unique_lock<std::mutex> lock(state->mtx);
        state->cv.wait(lock, [state] { return state->completed; });
    }
    
    // Small delay to ensure terminal is ready
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    
    // Enable colors if available
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Success
        init_pair(2, COLOR_RED, COLOR_BLACK);    // Failure
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Warning
    }
    
    // Get terminal dimensions
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Create notification window
    int win_height = 9;
    int win_width = 60;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;
    
    // Ensure window fits on screen
    if (win_width > max_x - 2) win_width = max_x - 2;
    if (win_height > max_y - 2) win_height = max_y - 2;
    if (start_x < 1) start_x = 1;
    if (start_y < 1) start_y = 1;
    
    WINDOW* win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    
    // Display notification content
    bool success = (state->exit_code == 0);
    
    if (has_colors()) {
        wattron(win, COLOR_PAIR(success ? 1 : 2) | A_BOLD);
    } else {
        wattron(win, A_BOLD);
    }
    
    mvwprintw(win, 1, 2, success ? "✓ COMMAND COMPLETED" : "✗ COMMAND FAILED");
    
    if (has_colors()) {
        wattroff(win, COLOR_PAIR(success ? 1 : 2) | A_BOLD);
    } else {
        wattroff(win, A_BOLD);
    }
    
    mvwprintw(win, 3, 2, "Command:");
    
    // Truncate command if too long
    std::string display_cmd = state->command;
    int max_cmd_len = win_width - 4;
    if (display_cmd.length() > (size_t)max_cmd_len) {
        display_cmd = display_cmd.substr(0, max_cmd_len - 3) + "...";
    }
    mvwprintw(win, 4, 2, "%s", display_cmd.c_str());
    
    mvwprintw(win, 5, 2, "Exit Code: %d", state->exit_code);
    
    wattron(win, A_DIM);
    mvwprintw(win, 7, 2, "Press any key to dismiss...");
    wattroff(win, A_DIM);
    
    wrefresh(win);
    
    // Wait for user input
    nodelay(win, FALSE);
    wgetch(win);
    
    // Cleanup
    delwin(win);
    endwin();
}

// Print usage information
void printUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <command> [args...]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Description:" << std::endl;
    std::cerr << "  Execute a command in the background without blocking the terminal." << std::endl;
    std::cerr << "  A notification will appear when the command completes." << std::endl;
    std::cerr << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "  " << program_name << " sleep 10" << std::endl;
    std::cerr << "  " << program_name << " wget https://example.com/file.zip" << std::endl;
    std::cerr << "  " << program_name << " make -j4" << std::endl;
}

int main(int argc, char* argv[]) {
    // Check arguments
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    // Build command string from arguments
    std::string command;
    for (int i = 1; i < argc; i++) {
        if (i > 1) command += " ";
        
        // Quote arguments if they contain spaces
        std::string arg = argv[i];
        if (arg.find(' ') != std::string::npos) {
            command += "\"" + arg + "\"";
        } else {
            command += arg;
        }
    }
    
    // Create shared state
    CommandState state;
    state.command = command;
    
    std::cout << "Starting background command: " << command << std::endl;
    std::cout << "You can continue using the terminal. A notification will appear when complete." << std::endl;
    
    // Fork process to fully detach from terminal
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "Error: Failed to fork process" << std::endl;
        return 1;
    }
    
    if (pid > 0) {
        // Parent process - exit immediately to return control to terminal
        return 0;
    }
    
    // Child process - run command and notification in background
    
    // Create new session to detach from terminal
    setsid();
    
    // Start command execution thread
    std::thread executor(executeCommand, &state);
    
    // Start monitoring thread
    std::thread monitor(monitorAndNotify, &state);
    
    // Wait for both threads to complete
    executor.join();
    monitor.join();
    
    return 0;
}
