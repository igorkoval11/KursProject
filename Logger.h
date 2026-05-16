#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

class Logger {
public:
    enum class Level { INFO, WARN, ERROR };
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
    static void setLogToConsole(bool enable);
    static void setLogToFile(bool enable);
    static void setMinLevel(Level level);
private:
    static Logger& getInstance();
    Logger() = default;
    ~Logger();
    void log(Level level, const std::string& msg);
    std::string getTimestamp();
    std::string levelToString(Level level);
    std::ofstream logFile;
    std::mutex mutex;
    bool logToConsole = true;
    bool logToFile = false;
    Level minLevel = Level::INFO;
};
