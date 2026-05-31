#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <map>
#include <vector>
#include <algorithm>

class Logger {
public:
    enum class Level { INFO, WARN, ERROR };

    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);

    static void setLogToConsole(bool enable);
    static void setLogToFile(bool enable);
    static void setMinLevel(Level level);

    // ── Статистика по шагу ──────────────────────────────────────────────────
    // Вызывается в начале каждого шага симуляции
    static void beginStep(int stepNumber);

    // Регистрирует событие в рамках текущего шага (не выводит в лог сразу)
    // type — произвольная метка (например "AirSensor", "TrafficLight", "WARN")
    static void recordEvent(const std::string& type);

    // Выводит сжатую строку-статистику по прошедшему шагу и сохраняет данные
    // для итоговой таблицы
    static void endStep();

    // ── Итоговая статистика ─────────────────────────────────────────────────
    // Выводит таблицу по всем шагам симуляции
    static void printSummary();

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
    bool logToFile    = false;
    Level minLevel    = Level::INFO;

    // текущий шаг
    int  currentStep = 0;
    std::map<std::string, int> stepEvents;   // тип → количество за шаг

    // история: stepNumber → map<type, count>
    std::vector<std::pair<int, std::map<std::string, int>>> history;
};