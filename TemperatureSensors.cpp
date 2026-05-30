//
// Created by Artemiy on 17.05.26.
//
#include "TemperatureSensors.h"
#include "Logger.h"
#include <random>

void TemperatureSensor::update() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10, 45);
    int temp = dis(gen);

    // 1) Сначала обновляем глобальную температуру
    getCurrentTemperature() = temp;

    // 2) Потом пересчитываем своё потребление на основе новой температуры
    refreshPowerConsumption();

    // 3) Накапливаем энергию
    consumeEnergy();

    Logger::info("[" + name + "] Температура: " + std::to_string(temp) +
                 "°C | Потребление: " + std::to_string(power_consumption) + " Вт" +
                 " (база " + std::to_string(BASE_POWER) + " Вт, отклонение " +
                 std::to_string(std::abs(temp - 20)) + "°C от нормы)");

    if (temp < -5)
        Logger::warn("[" + name + "] Слишком холодно (" + std::to_string(temp) + "°C)");
    else if (temp > 35)
        Logger::warn("[" + name + "] Перегрев (" + std::to_string(temp) + "°C)");
}

void TemperatureSensor::turnOff() {
    // Датчик температуры нельзя отключить
}

bool TemperatureSensor::isOn() const {
    return true;
}

bool TemperatureSensor::canBeTurnedOff() const {
    return false;
}