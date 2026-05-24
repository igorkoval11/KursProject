//
// Created by Artemiy on 2.05.26.
//
#ifndef KURSPROJECT_TRAFFICLIGHT_H
#define KURSPROJECT_TRAFFICLIGHT_H
#include "IoTDevice.h"
#include <string>
#include <vector>
#include <random>

class TrafficLight : public IoTDevice {
public:
    enum class Phase { RED, YELLOW, GREEN };

    struct PhaseConfig {
        Phase phase;
        int ticks; // сколько тиков держать фазу
    };

private:
    std::vector<PhaseConfig> schedule; // расписание фаз
    size_t currentPhaseIndex;          // индекс текущей фазы
    int phaseTimer;                    // сколько тиков прошло в текущей фазе

public:
    // Конструктор с расписанием
    TrafficLight(std::string name, int power = 20,
                 std::vector<PhaseConfig> config = {})
        : IoTDevice(std::move(name), power, 1),
          schedule(config),
          currentPhaseIndex(0),
          phaseTimer(0) {
        // Если расписание не задано — стандартное
        if (schedule.empty()) {
            schedule = {
                {Phase::RED, 2},
                {Phase::GREEN, 2},
                {Phase::YELLOW, 1}
            };
        }
        // Случайная начальная фаза
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 2);
        currentPhaseIndex = dis(gen) % schedule.size();
    }

    void update() override;
    void turnOff() override;
    bool isOn() const override;

private:
    std::string phaseToString(Phase phase) const;
};

#endif
