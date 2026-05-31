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
        int ticks;
    };

private:
    std::vector<PhaseConfig> schedule;
    size_t currentPhaseIndex;
    int phaseTimer;

public:
    TrafficLight(std::string name, int power = 20,
                 std::vector<PhaseConfig> config = {})
        : IoTDevice(std::move(name), power, 1),
          schedule(config.empty() ? std::vector<PhaseConfig>{
              {Phase::RED, 2}, {Phase::GREEN, 2}, {Phase::YELLOW, 1}
          } : config),
          currentPhaseIndex(0),
          phaseTimer(0) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 2);
        currentPhaseIndex = dis(gen) % schedule.size();
    }

    void update() override;
    void turnOff() override;
    bool isOn() const override;

    Phase getCurrentPhase() const {
        return schedule[currentPhaseIndex].phase;
    }

    // Публичный метод для использования в AirSensor
    std::string phaseToString(Phase phase) const;

    // Потребление фиксированное — не зависит от температуры
    void refreshPowerConsumption() {
        power_consumption = base_power;
    }
};

#endif