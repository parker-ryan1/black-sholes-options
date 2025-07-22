#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <thread>

/**
 * @file logger.hpp
 * @brief Thread-safe logging system for quantitative finance applications
 * @author Quantitative Finance Team
 * @version 1.0
 * @date 2025
 * 
 * This logging system provides:
 * - Thread-safe logging operations
 * - Multiple log levels (DEBUG, INFO, WARNING, ERROR, CRITICAL)
 * - Configurable output destinations (console, file, both)
 * - Automatic log rotation
 * - Performance monitoring integration
 * - Memory-efficient string formatting
 */

namespace Utils {

/**
 * @brief Log severity levels
 */
enum class LogLevel {
    DEBUG = 0,      ///< Detailed information for debugging
    INFO = 1,       ///< General information about program execution
    WARNING = 2,    ///< Warning messages for potential issues
    ERROR = 3,      ///< Error messages for recoverable errors
    CRITICAL = 4    ///< Critical errors that may cause program termination
};

/**
 * @brief Convert log level to string representation
 * @param level Log level to convert
 * @return String representation of log level
 */
std::string to_string(LogLevel level);

/**
 * @brief Thread-safe logger class
 * 
 * This logger provides thread-safe logging capabilities with configurable
 * output destinations and automatic timestamping. It supports both
 * synchronous and asynchronous logging modes.
 */
class Logger {
private:
    std::string component_name_;        ///< Component name for log identification
    static std::mutex global_mutex_;    ///< Global mutex for thread safety
    static std::ofstream log_file_;     ///< Global log file stream
    static LogLevel min_level_;         ///< Minimum log level to output
    static bool console_output_;        ///< Whether to output to console
    static bool file_output_;           ///< Whether to output to file
    static std::string log_filename_;   ///< Current log file name
    static size_t max_file_size_;       ///< Maximum log file size before rotation
    static size_t current_file_size_;   ///< Current log file size
    static int max_log_files_;          ///< Maximum number of log files to keep
    
    /**
     * @brief Get current timestamp as string
     * @return Formatted timestamp string
     */
    static std::string get_timestamp();
    
    /**
     * @brief Get current thread ID as string
     * @return Thread ID string
     */
    static std::string get_thread_id();
    
    /**
     * @brief Rotate log files when size limit is reached
     */
    static void rotate_log_files();
    
    /**
     * @brief Write log message to destinations
     * @param level Log level
     * @param message Formatted message
     */
    void write_log(LogLevel level, const std::string& message);

public:
    /**
     * @brief Construct logger for specific component
     * @param component_name Name of the component using this logger
     */
    explicit Logger(const std::string& component_name);
    
    /**
     * @brief Destructor - ensures all logs are flushed
     */
    ~Logger();
    
    // Disable copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Enable move constructor and assignment operator
    Logger(Logger&&) = default;
    Logger& operator=(Logger&&) = default;
    
    /**
     * @brief Configure global logger settings
     * @param min_level Minimum log level to output
     * @param console_output Enable console output
     * @param file_output Enable file output
     * @param log_filename Log file name (if file output enabled)
     * @param max_file_size Maximum file size before rotation (bytes)
     * @param max_log_files Maximum number of log files to keep
     */
    static void configure(
        LogLevel min_level = LogLevel::INFO,
        bool console_output = true,
        bool file_output = true,
        const std::string& log_filename = "quantlib.log",
        size_t max_file_size = 10 * 1024 * 1024,  // 10MB
        int max_log_files = 5
    );
    
    /**
     * @brief Log debug message
     * @param format Format string (printf-style)
     * @param args Format arguments
     */
    template<typename... Args>
    void debug(const std::string& format, Args&&... args) {
        if (min_level_ <= LogLevel::DEBUG) {
            log(LogLevel::DEBUG, format, std::forward<Args>(args)...);
        }
    }
    
    /**
     * @brief Log info message
     * @param format Format string (printf-style)
     * @param args Format arguments
     */
    template<typename... Args>
    void info(const std::string& format, Args&&... args) {
        if (min_level_ <= LogLevel::INFO) {
            log(LogLevel::INFO, format, std::forward<Args>(args)...);
        }
    }
    
    /**
     * @brief Log warning message
     * @param format Format string (printf-style)
     * @param args Format arguments
     */
    template<typename... Args>
    void warning(const std::string& format, Args&&... args) {
        if (min_level_ <= LogLevel::WARNING) {
            log(LogLevel::WARNING, format, std::forward<Args>(args)...);
        }
    }
    
    /**
     * @brief Log error message
     * @param format Format string (printf-style)
     * @param args Format arguments
     */
    template<typename... Args>
    void error(const std::string& format, Args&&... args) {
        if (min_level_ <= LogLevel::ERROR) {
            log(LogLevel::ERROR, format, std::forward<Args>(args)...);
        }
    }
    
    /**
     * @brief Log critical message
     * @param format Format string (printf-style)
     * @param args Format arguments
     */
    template<typename... Args>
    void critical(const std::string& format, Args&&... args) {
        if (min_level_ <= LogLevel::CRITICAL) {
            log(LogLevel::CRITICAL, format, std::forward<Args>(args)...);
        }
    }
    
    /**
     * @brief Flush all pending log messages
     */
    static void flush();
    
    /**
     * @brief Get current log level
     * @return Current minimum log level
     */
    static LogLevel get_level() { return min_level_; }
    
    /**
     * @brief Check if level is enabled for logging
     * @param level Log level to check
     * @return true if level will be logged
     */
    static bool is_enabled(LogLevel level) { return level >= min_level_; }

private:
    /**
     * @brief Internal logging function with formatting
     * @param level Log level
     * @param format Format string
     * @param args Format arguments
     */
    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args) {
        try {
            std::ostringstream oss;
            format_string(oss, format, std::forward<Args>(args)...);
            write_log(level, oss.str());
        } catch (const std::exception& e) {
            // Fallback logging in case of formatting errors
            write_log(LogLevel::ERROR, "Log formatting error: " + std::string(e.what()));
        }
    }
    
    /**
     * @brief Format string with arguments (recursive base case)
     * @param oss Output string stream
     * @param format Format string
     */
    void format_string(std::ostringstream& oss, const std::string& format);
    
    /**
     * @brief Format string with arguments (recursive case)
     * @param oss Output string stream
     * @param format Format string
     * @param arg First argument
     * @param args Remaining arguments
     */
    template<typename T, typename... Args>
    void format_string(std::ostringstream& oss, const std::string& format, T&& arg, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos != std::string::npos) {
            oss << format.substr(0, pos) << std::forward<T>(arg);
            format_string(oss, format.substr(pos + 2), std::forward<Args>(args)...);
        } else {
            oss << format;
        }
    }
};

/**
 * @brief RAII class for performance timing
 * 
 * This class automatically logs the execution time of a code block
 * when it goes out of scope.
 */
class PerformanceTimer {
private:
    Logger& logger_;
    std::string operation_name_;
    std::chrono::high_resolution_clock::time_point start_time_;
    LogLevel log_level_;

public:
    /**
     * @brief Start performance timer
     * @param logger Logger instance to use
     * @param operation_name Name of operation being timed
     * @param log_level Log level for timing message
     */
    PerformanceTimer(Logger& logger, const std::string& operation_name, 
                    LogLevel log_level = LogLevel::DEBUG);
    
    /**
     * @brief Destructor - logs elapsed time
     */
    ~PerformanceTimer();
    
    /**
     * @brief Get elapsed time without stopping timer
     * @return Elapsed time in milliseconds
     */
    double elapsed_ms() const;
};

/**
 * @brief Macro for easy performance timing
 * 
 * Usage: PERF_TIMER(logger, "operation_name");
 */
#define PERF_TIMER(logger, name) \
    Utils::PerformanceTimer _perf_timer(logger, name)

/**
 * @brief Macro for performance timing with custom log level
 */
#define PERF_TIMER_LEVEL(logger, name, level) \
    Utils::PerformanceTimer _perf_timer(logger, name, level)

} // namespace Utils

// Global logger instance for convenience
extern Utils::Logger g_logger;