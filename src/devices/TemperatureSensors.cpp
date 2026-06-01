//
// Created by Artemiy on 17.05.26.
//
#include "devices/TemperatureSensors.h"
#include "utils/Logger.h"
#include <random>
void TemperatureSensor::update() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10, 45);
    int temp = dis(gen);
    
    // Обновляем текущую температуру в системе
    getCurrentTemperature() = temp;
    
    consumeEnergy();
    
    Logger::info("[" + name + "] Температура: " + std::to_string(temp) +
                 "°C | Потребление: " + std::to_string(power_consumption) + " Вт");
    if (temp < -5)
        Logger::warn("[" + name + "] Слишком холодно (" + std::to_string(temp) + "°C)");
    else if (temp > 35)
        Logger::warn("[" + name + "] Перегрев (" + std::to_string(temp) + "°C)");
}

void TemperatureSensor::turnOff() {
    // Датчик температуры нельзя отключить ни при каком сценарии
}

bool TemperatureSensor::isOn() const {
    return true;
}

bool TemperatureSensor::canBeTurnedOff() const {
    return false;
}