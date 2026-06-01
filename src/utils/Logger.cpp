#include "include/utils/Logger.h"
#include <vector>
#include <algorithm>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (logFile.is_open()) logFile.close();
}

void Logger::info(const std::string& msg)  { getInstance().log(Level::INFO,  msg); }
void Logger::warn(const std::string& msg)  { getInstance().log(Level::WARN,  msg); }
void Logger::error(const std::string& msg) { getInstance().log(Level::ERROR, msg); }

void Logger::log(Level level, const std::string& msg) {
    if (level < minLevel) return;
    std::lock_guard<std::mutex> lock(mutex);
    std::string formattedMsg =
        "[" + levelToString(level) + "] [" + getTimestamp() + "] " + msg;
    if (logToConsole) {
        if (level == Level::ERROR) std::cerr << formattedMsg << std::endl;
        else                       std::cout << formattedMsg << std::endl;
    }
    if (logToFile && logFile.is_open()) {
        logFile << formattedMsg << std::endl;
        logFile.flush();
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now.time_since_epoch()) % 1000;
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%H:%M:%S")
       << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::INFO:  return "INFO";
        case Level::WARN:  return "WARN";
        case Level::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::setLogToConsole(bool enable) { getInstance().logToConsole = enable; }

void Logger::setLogToFile(bool enable) {
    auto& inst = getInstance();
    if (enable && !inst.logFile.is_open()) {
        inst.logFile.open("../log.txt", std::ios::out | std::ios::app);
    } else if (!enable && inst.logFile.is_open()) {
        inst.logFile.close();
    }
    inst.logToFile = enable;
}

void Logger::setMinLevel(Level level) { getInstance().minLevel = level; }

// ── Статистика по шагу ──────────────────────────────────────────────────────

void Logger::beginStep(int stepNumber) {
    auto& inst = getInstance();
    std::lock_guard<std::mutex> lock(inst.mutex);
    inst.currentStep = stepNumber;
    inst.stepEvents.clear();
}

void Logger::recordEvent(const std::string& type) {
    auto& inst = getInstance();
    std::lock_guard<std::mutex> lock(inst.mutex);
    inst.stepEvents[type]++;
}

void Logger::endStep() {
    auto& inst = getInstance();
    std::lock_guard<std::mutex> lock(inst.mutex);

    // Формируем сжатую строку: "Шаг N: AirSensor×20  TrafficLight×10 ..."
    std::ostringstream summary;
    summary << "  Итог шага " << inst.currentStep << ":";
    int total = 0;
    for (const auto& kv : inst.stepEvents) {
        summary << "  " << kv.first << "×" << kv.second;
        total += kv.second;
    }
    summary << "  | всего событий: " << total;

    std::string line = "[STEP] [" + inst.getTimestamp() + "] " + summary.str();
    if (inst.logToConsole) std::cout << line << std::endl;
    if (inst.logToFile && inst.logFile.is_open()) {
        inst.logFile << line << std::endl;
        inst.logFile.flush();
    }

    // Сохраняем для printSummary
    inst.history.push_back({inst.currentStep, inst.stepEvents});
}

// ── Итоговая статистика ──────────────────────────────────────────────────────

void Logger::printSummary() {
    auto& inst = getInstance();
    std::lock_guard<std::mutex> lock(inst.mutex);

    // Собираем все типы событий
    std::vector<std::string> types;
    for (const auto& entry : inst.history) {
        for (const auto& kv : entry.second) {
            if (std::find(types.begin(), types.end(), kv.first) == types.end())
                types.push_back(kv.first);
        }
    }
    std::sort(types.begin(), types.end());

    // Ширина колонки
    const int COL = 14;

    auto printLine = [&](std::ostream& os) {
        os << std::string(6 + COL * (int)(types.size() + 1), '-') << "\n";
    };

    auto outputSummary = [&](std::ostream& os) {
        os << "\n========== ИТОГОВАЯ СТАТИСТИКА СИМУЛЯЦИИ ==========\n";

        // Заголовок
        os << std::left << std::setw(6) << "Шаг";
        for (const auto& t : types)
            os << std::setw(COL) << t;
        os << std::setw(COL) << "ВСЕГО" << "\n";
        printLine(os);

        // Строки + накопленные суммы
        std::map<std::string, int> totals;
        for (const auto& entry : inst.history) {
            int step  = entry.first;
            const auto& ev = entry.second;
            int rowTotal = 0;
            os << std::setw(6) << step;
            for (const auto& t : types) {
                int cnt = ev.count(t) ? ev.at(t) : 0;
                os << std::setw(COL) << cnt;
                totals[t] += cnt;
                rowTotal  += cnt;
            }
            os << std::setw(COL) << rowTotal << "\n";
        }

        printLine(os);

        // Итоговая строка
        int grandTotal = 0;
        os << std::setw(6) << "ΣΣΣ";
        for (const auto& t : types) {
            os << std::setw(COL) << totals[t];
            grandTotal += totals[t];
        }
        os << std::setw(COL) << grandTotal << "\n";
        os << "====================================================\n\n";
    };

    if (inst.logToConsole) outputSummary(std::cout);
    if (inst.logToFile && inst.logFile.is_open()) {
        outputSummary(inst.logFile);
        inst.logFile.flush();
    }
}