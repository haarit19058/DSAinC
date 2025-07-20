#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <iomanip> // For std::setw, std::setprecision, etc.

// Headers for process management and resource limits on UNIX-like systems.
// This implementation is POSIX-specific.
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>

namespace fs = std::filesystem;

// ANSI color codes for rich terminal output.
const char* const RESET_COLOR = "\033[0m";
const char* const GREEN_COLOR = "\033[32m";
const char* const RED_COLOR = "\033[31m";
const char* const YELLOW_COLOR = "\033[33m";
const char* const BLUE_COLOR = "\033[34m";
const char* const MAGENTA_COLOR = "\033[35m";
const char* const CYAN_COLOR = "\033[36m";

/**
 * @brief Holds all configuration for the test harness.
 */
struct Config {
    std::string source_file;
    std::string test_cases_path;
    std::string binary_name = "solution";
    std::string compiler = "g++";
    std::string cpp_flags = "-std=c++17 -O2 -Wall";
    std::string temp_output_file = "tmp.out";
    std::string compiler_log_file = "compile_errors.log";

    // Resource limits
    int time_limit_seconds = 10;
    rlim_t memory_limit_mb = 1024; // 1 GB
};

/**
 * @brief RAII-based cleanup helper to ensure temporary files are removed.
 */
struct CleanupGuard {
    const Config& cfg;
    ~CleanupGuard() {
        fs::remove(cfg.binary_name);
        fs::remove(cfg.temp_output_file);
        fs::remove(cfg.compiler_log_file);
    }
};

/**
 * @brief Compares two files line by line, ignoring trailing whitespace.
 */
bool compare_files(const std::string& produced_path, const std::string& expected_path, std::string& diff_details) {
    std::ifstream f_produced(produced_path), f_expected(expected_path);
    std::string line_produced, line_expected;
    int line_num = 1;

    while (true) {
        bool has_produced = static_cast<bool>(std::getline(f_produced, line_produced));
        bool has_expected = static_cast<bool>(std::getline(f_expected, line_expected));

        if (has_produced) line_produced.erase(line_produced.find_last_not_of(" \n\r\t") + 1);
        if (has_expected) line_expected.erase(line_expected.find_last_not_of(" \n\r\t") + 1);

        if (!has_produced && !has_expected) return true;

        if (line_produced != line_expected) {
            std::ostringstream oss;
            oss << "Difference at Line " << line_num << ":\n"
                << RED_COLOR << "  Expected: `" << line_expected << "`\n"
                << GREEN_COLOR << "  Got:      `" << line_produced << "`" << RESET_COLOR;
            diff_details = oss.str();
            return false;
        }
        line_num++;
    }
}

/**
 * @brief Compiles the source file specified in the configuration.
 */
bool compile(const Config& cfg) {
    std::cout << BLUE_COLOR << "Compiling " << cfg.source_file << "..." << RESET_COLOR << std::endl;
    std::string command = cfg.compiler + " " + cfg.cpp_flags + " " + cfg.source_file + " -o " + cfg.binary_name + " 2> " + cfg.compiler_log_file;

    int compile_status = system(command.c_str());

    if (compile_status != 0) {
        std::cerr << RED_COLOR << "Compilation failed. Errors logged to " << cfg.compiler_log_file << RESET_COLOR << std::endl;
        std::ifstream error_file(cfg.compiler_log_file);
        if (error_file) {
            std::stringstream buffer;
            buffer << error_file.rdbuf();
            std::cerr << buffer.str() << std::endl;
        }
        return false;
    }

    std::cout << GREEN_COLOR << "Compilation successful." << RESET_COLOR << std::endl;
    return true;
}

/**
 * @brief Executes the user's solution and reports status, time, and memory.
 *
 * This function uses fork() to create a child process. The child sets resource limits
 * and runs the user's code. The parent waits using wait4(), which provides detailed
 * resource usage statistics for the completed child process.
 */
void run_test_case(const fs::path& in_path, const Config& cfg, int& passed_count) {
    std::string base_name = in_path.stem().string();
    fs::path expected_out_path = in_path;
    expected_out_path.replace_extension(".out");

    std::cout << YELLOW_COLOR << "Running test: " << std::left << std::setw(15) << base_name << RESET_COLOR << " ... ";
    
    if (!fs::exists(expected_out_path)) {
        std::cout << MAGENTA_COLOR << "SKIPPED" << RESET_COLOR << " (Missing .out file)" << std::endl;
        return;
    }
    
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << RED_COLOR << "FATAL: fork() failed." << RESET_COLOR << std::endl;
        return;
    }

    if (pid == 0) { // Child process
        // 1. Redirect I/O
        if (!freopen(in_path.c_str(), "r", stdin) || !freopen(cfg.temp_output_file.c_str(), "w", stdout)) {
             perror("freopen");
             exit(127);
        }
        
        // 2. Set resource limits
        struct rlimit time_limit, mem_limit;
        time_limit.rlim_cur = cfg.time_limit_seconds;
        time_limit.rlim_max = cfg.time_limit_seconds + 1;
        setrlimit(RLIMIT_CPU, &time_limit);
        
        mem_limit.rlim_cur = cfg.memory_limit_mb * 1024 * 1024;
        mem_limit.rlim_max = cfg.memory_limit_mb * 1024 * 1024;
        setrlimit(RLIMIT_AS, &mem_limit);

        // 3. Execute the user's program
        execl(("./" + cfg.binary_name).c_str(), cfg.binary_name.c_str(), nullptr);
        
        perror("execl");
        exit(127);

    } else { // Parent process
        int status;
        struct rusage usage;
        
        // Use wait4 to get resource usage from the child
        wait4(pid, &status, 0, &usage);

        // Calculate CPU time and memory usage
        double cpu_time_ms = (usage.ru_utime.tv_sec * 1000.0 + usage.ru_utime.tv_usec / 1000.0) +
                             (usage.ru_stime.tv_sec * 1000.0 + usage.ru_stime.tv_usec / 1000.0);
        // ru_maxrss is in kilobytes on Linux
        long memory_kb = usage.ru_maxrss;

        std::stringstream stats_ss;
        stats_ss << std::fixed << std::setprecision(2)
                 << "(Time: " << cpu_time_ms << " ms, "
                 << "Mem: " << memory_kb << " KB)";

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 0) {
                std::string diff_details;
                if (compare_files(cfg.temp_output_file, expected_out_path.string(), diff_details)) {
                    std::cout << GREEN_COLOR << std::left << std::setw(12) << "PASS" << RESET_COLOR << CYAN_COLOR << stats_ss.str() << RESET_COLOR << std::endl;
                    passed_count++;
                } else {
                    std::cout << RED_COLOR << std::left << std::setw(12) << "FAIL" << RESET_COLOR << CYAN_COLOR << stats_ss.str() << RESET_COLOR << std::endl;
                    std::cout << diff_details << std::endl;
                }
            } else {
                std::cout << RED_COLOR << std::left << std::setw(12) << "RE" << RESET_COLOR << " (Exit Code: " << exit_code << ") " << CYAN_COLOR << stats_ss.str() << RESET_COLOR << std::endl;
            }
        } else if (WIFSIGNALED(status)) {
            int term_signal = WTERMSIG(status);
            if (term_signal == SIGXCPU) {
                std::cout << RED_COLOR << std::left << std::setw(12) << "TLE" << RESET_COLOR << " (Time Limit Exceeded) " << CYAN_COLOR << stats_ss.str() << RESET_COLOR << std::endl;
            } else if (term_signal == SIGSEGV || term_signal == SIGABRT) {
                std::cout << RED_COLOR << std::left << std::setw(12) << "MLE/RE" << RESET_COLOR << " (Segfault/Abort) " << CYAN_COLOR << stats_ss.str() << RESET_COLOR << std::endl;
            } else {
                std::cout << RED_COLOR << std::left << std::setw(12) << "RE" << RESET_COLOR << " (Signal: " << term_signal << ") " << CYAN_COLOR << stats_ss.str() << RESET_COLOR << std::endl;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file.cpp> [test_cases_directory]" << std::endl;
        return 1;
    }

    Config cfg;
    cfg.source_file = argv[1];

    if (argc >= 3) {
        cfg.test_cases_path = argv[2];
    } else {
        cfg.test_cases_path = "/home/haarit/testCasesCP";
        std::cout << YELLOW_COLOR << "No test case directory provided. Using default: " 
                  << cfg.test_cases_path << RESET_COLOR << std::endl;
    }

    if (!fs::exists(cfg.source_file)) {
        std::cerr << RED_COLOR << "Error: Source file not found: " << cfg.source_file << RESET_COLOR << std::endl;
        return 1;
    }
    if (!fs::is_directory(cfg.test_cases_path)) {
        std::cerr << RED_COLOR << "Error: Test cases directory not found: " << cfg.test_cases_path << RESET_COLOR << std::endl;
        return 1;
    }

    CleanupGuard cleanup_guard{cfg};

    if (!compile(cfg)) {
        return 1;
    }

    std::vector<fs::path> test_files;
    for (const auto& entry : fs::directory_iterator(cfg.test_cases_path)) {
        if (entry.path().extension() == ".in") {
            test_files.push_back(entry.path());
        }
    }
    
    std::sort(test_files.begin(), test_files.end(), [](const fs::path& a, const fs::path& b){
        std::string s_a = a.stem().string();
        std::string s_b = b.stem().string();
        try {
            int num_a, num_b;
            size_t pos_a, pos_b;
            num_a = std::stoi(s_a, &pos_a);
            num_b = std::stoi(s_b, &pos_b);
            // If the entire string is a number, compare numerically
            if (pos_a == s_a.length() && pos_b == s_b.length()) {
                 return num_a < num_b;
            }
        } catch(const std::exception&) {
             // Fallback for non-numeric filenames
        }
        return s_a < s_b; // Natural string comparison
    });
    
    int passed_count = 0;
    int total_tests = test_files.size();
    
    if (total_tests == 0) {
        std::cout << YELLOW_COLOR << "No '.in' files found in the test directory." << RESET_COLOR << std::endl;
    }

    for (const auto& in_path : test_files) {
        run_test_case(in_path, cfg, passed_count);
    }

    std::cout << "\n" << BLUE_COLOR << "------ Summary ------" << RESET_COLOR << std::endl;
    double percentage = (total_tests > 0) ? (static_cast<double>(passed_count) / total_tests) * 100.0 : 0.0;
    std::cout << "Tests Passed: " << (passed_count == total_tests ? GREEN_COLOR : RED_COLOR)
              << passed_count << " / " << total_tests << RESET_COLOR 
              << " (" << std::fixed << std::setprecision(2) << percentage << "%)" << std::endl;
    std::cout << BLUE_COLOR << "---------------------" << RESET_COLOR << std::endl;

    return (passed_count == total_tests) ? 0 : 1;
}
