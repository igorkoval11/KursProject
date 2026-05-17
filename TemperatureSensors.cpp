//
// Created by Artemiy on 17.05.26.
//
#include "TemperatureSensors.h"
#include "Logger.h"
#include <random>
void TemperatureSensor::update() {
    if (!isOn()) return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10, 45);
    int temp = dis(gen);
    // чем холоднее — тем выше нагрузка
    // -10°C → 25 Вт  |  20°C → 10 Вт  |  45°C → 10 Вт
    int dynamicPower = 10 + std::max(0, (20 - temp) / 2);
    power_consumption = dynamicPower;
    consumeEnergy();
    Logger::info("[" + name + "] Температура: " + std::to_string(temp) +
                 "°C | Потребление: " + std::to_string(dynamicPower) + " Вт");
    if (temp < -5)
        Logger::warn("[" + name + "] Слишком холодно (" + std::to_string(temp) + "°C)");
    else if (temp > 35)
        Logger::warn("[" + name + "] Перегрев (" + std::to_string(temp) + "°C)");
}