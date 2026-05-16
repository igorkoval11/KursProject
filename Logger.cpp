#include "Logger.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (logFile.is_open()) logFile.close();
}

void Logger::info(const std::string& msg) { getInstance().log(Level::INFO, msg); }
void Logger::warn(const std::string& msg) { getInstance().log(Level::WARN, msg); }
void Logger::error(const std::string& msg) { getInstance().log(Level::ERROR, msg); }

void Logger::log(Level level, const std::string& msg) {
    if (level < minLevel) return;
    std::lock_guard<std::mutex> lock(mutex);
    std::string formattedMsg = "[" + levelToString(level) + "] [" + getTimestamp() + "] " + msg;
    if (logToConsole) {
        if (level == Level::ERROR) std::cerr << formattedMsg << std::endl;
        else std::cout << formattedMsg << std::endl;
    }
    if (logToFile && logFile.is_open()) {
        logFile << formattedMsg << std::endl;
        logFile.flush();
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::INFO: return "INFO";
        case Level::WARN: return "WARN";
        case Level::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::setLogToConsole(bool enable) { getInstance().logToConsole = enable; }

void Logger::setLogToFile(bool enable) {
    auto& inst = getInstance();
    if (enable && !inst.logFile.is_open()) {
        inst.logFile.open("log.txt", std::ios::out | std::ios::app);
    } else if (!enable && inst.logFile.is_open()) {
        inst.logFile.close();
    }
    inst.logToFile = enable;
}

void Logger::setMinLevel(Level level) { getInstance().minLevel = level; }
