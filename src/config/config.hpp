#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <fstream>
#include "../utils/logger.hpp"

/**
 * @file config.hpp
 * @brief Configuration management system for quantitative finance applications
 * @author Quantitative Finance Team
 * @version 1.0
 * @date 2025
 * 
 * This configuration system provides:
 * - Thread-safe configuration loading and access
 * - JSON-based configuration files
 * - Runtime configuration updates
 * - Environment variable overrides
 * - Configuration validation
 * - Default value management
 */

namespace Config {

/**
 * @brief Configuration value types
 */
enum class ValueType {
    STRING,
    INTEGER,
    DOUBLE,
    BOOLEAN
};

/**
 * @brief Configuration value wrapper
 */
class ConfigValue {
private:
    std::string string_value_;
    ValueType type_;
    
public:
    ConfigValue() : type_(ValueType::STRING) {}
    explicit ConfigValue(const std::string& value) : string_value_(value), type_(ValueType::STRING) {}
    explicit ConfigValue(int value) : string_value_(std::to_string(value)), type_(ValueType::INTEGER) {}
    explicit ConfigValue(double value) : string_value_(std::to_string(value)), type_(ValueType::DOUBLE) {}
    explicit ConfigValue(bool value) : string_value_(value ? "true" : "false"), type_(ValueType::BOOLEAN) {}
    
    // Conversion operators
    operator std::string() const { return string_value_; }
    operator int() const { return std::stoi(string_value_); }
    operator double() const { return std::stod(string_value_); }
    operator bool() const { return string_value_ == "true" || string_value_ == "1"; }
    
    ValueType getType() const { return type_; }
    const std::string& getString() const { return string_value_; }
};

/**
 * @brief Thread-safe configuration manager (Singleton)
 * 
 * This class manages application configuration with support for:
 * - JSON configuration files
 * - Environment variable overrides
 * - Runtime configuration updates
 * - Thread-safe access
 * - Configuration validation
 */
class ConfigManager {
private:
    mutable std::mutex mutex_;
    std::map<std::string, ConfigValue> config_map_;
    std::string config_file_path_;
    Utils::Logger logger_;
    
    // Private constructor for singleton
    ConfigManager();
    
    /**
     * @brief Load configuration from JSON file
     * @param file_path Path to configuration file
     * @return true if successful
     */
    bool loadFromFile(const std::string& file_path);
    
    /**
     * @brief Load environment variable overrides
     */
    void loadEnvironmentOverrides();
    
    /**
     * @brief Parse JSON configuration string
     * @param json_content JSON content as string
     * @return true if successful
     */
    bool parseJsonContent(const std::string& json_content);
    
    /**
     * @brief Set default configuration values
     */
    void setDefaults();
    
    /**
     * @brief Validate configuration values
     * @return true if all values are valid
     */
    bool validateConfiguration();

public:
    // Singleton access
    static ConfigManager& getInstance();
    
    // Disable copy constructor and assignment
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    /**
     * @brief Initialize configuration system
     * @param config_file_path Path to configuration file (optional)
     * @return true if initialization successful
     */
    bool initialize(const std::string& config_file_path = "config.json");
    
    /**
     * @brief Get configuration value as string
     * @param key Configuration key
     * @param default_value Default value if key not found
     * @return Configuration value
     */
    std::string getString(const std::string& key, const std::string& default_value = "") const;
    
    /**
     * @brief Get configuration value as integer
     * @param key Configuration key
     * @param default_value Default value if key not found
     * @return Configuration value
     */
    int getInt(const std::string& key, int default_value = 0) const;
    
    /**
     * @brief Get configuration value as double
     * @param key Configuration key
     * @param default_value Default value if key not found
     * @return Configuration value
     */
    double getDouble(const std::string& key, double default_value = 0.0) const;
    
    /**
     * @brief Get configuration value as boolean
     * @param key Configuration key
     * @param default_value Default value if key not found
     * @return Configuration value
     */
    bool getBool(const std::string& key, bool default_value = false) const;
    
    /**
     * @brief Set configuration value
     * @param key Configuration key
     * @param value Configuration value
     */
    void set(const std::string& key, const ConfigValue& value);
    
    /**
     * @brief Check if configuration key exists
     * @param key Configuration key
     * @return true if key exists
     */
    bool hasKey(const std::string& key) const;
    
    /**
     * @brief Save current configuration to file
     * @param file_path Output file path (optional, uses current config file if empty)
     * @return true if successful
     */
    bool saveToFile(const std::string& file_path = "") const;
    
    /**
     * @brief Reload configuration from file
     * @return true if successful
     */
    bool reload();
    
    /**
     * @brief Get all configuration keys
     * @return Vector of all configuration keys
     */
    std::vector<std::string> getAllKeys() const;
    
    /**
     * @brief Print current configuration to log
     * @param log_level Log level to use
     */
    void printConfiguration(Utils::LogLevel log_level = Utils::LogLevel::INFO) const;
    
    // Convenience methods for common configuration values
    int getMonteCarloSimulations() const { return getInt("monte_carlo.simulations", 100000); }
    int getMonteCarloSteps() const { return getInt("monte_carlo.steps", 252); }
    double getImpliedVolTolerance() const { return getDouble("implied_vol.tolerance", 1e-6); }
    int getImpliedVolMaxIterations() const { return getInt("implied_vol.max_iterations", 100); }
    bool getEnablePerformanceLogging() const { return getBool("performance.enable_logging", true); }
    std::string getLogLevel() const { return getString("logging.level", "INFO"); }
    std::string getLogFile() const { return getString("logging.file", "quantlib.log"); }
    bool getLogToConsole() const { return getBool("logging.console", true); }
    bool getLogToFile() const { return getBool("logging.file_output", true); }
    int getMaxLogFiles() const { return getInt("logging.max_files", 5); }
    size_t getMaxLogFileSize() const { return static_cast<size_t>(getInt("logging.max_file_size_mb", 10)) * 1024 * 1024; }
    
    // Thread safety settings
    bool getEnableThreadSafety() const { return getBool("threading.enable_safety", true); }
    int getMaxThreads() const { return getInt("threading.max_threads", std::thread::hardware_concurrency()); }
    
    // Memory management settings
    bool getEnableMemoryProfiling() const { return getBool("memory.enable_profiling", false); }
    size_t getMaxMemoryUsageMB() const { return static_cast<size_t>(getInt("memory.max_usage_mb", 1024)); }
    
    // Numerical precision settings
    double getNumericalTolerance() const { return getDouble("numerical.tolerance", 1e-12); }
    int getMaxIterations() const { return getInt("numerical.max_iterations", 1000); }
};

// Global configuration instance for convenience
#define Config ConfigManager::getInstance()

} // namespace Config