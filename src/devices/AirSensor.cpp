//
// Created by matvl on 04.05.2026.
//
#include "devices/AirSensor.h"
#include "utils/Logger.h"
#include <random>

void AirSensor::update() {
    if (!isOn()) return;

    consumeEnergy();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    int level = dis(gen);
    Logger::info("Уровень грязи: " + std::to_string(level) + "%");

    if (level > alarmThreshold) {
        Logger::warn("ALARM");
    }
}
