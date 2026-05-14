//
// Created by matvl on 04.05.2026.
//

#include "AirSensor.h"
#include "Logger.h"
#include <random>

void AirSensor::update() {
    if (!isOn()) return;

    consumeEnergy();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    int level = dis(gen);
    Logger::info("Уровень грязи: " + std::to_string(level) + "%");

    if (level > 70) {
        Logger::warn("ALARM");  
    }
}