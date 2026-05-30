//
// Created by matvl on 03.05.2026.
//

#include "HUB2.h"
#include "Logger.h"
#include "TemperatureSensors.h"
#include "AirSensor.h"
#include "TrafficLight.h"

#include <string>
#include <unordered_map>

int Hub2::extractNumber(const std::string& name) {
    // Ищем последний '-' и берём всё после него как число
    auto pos = name.rfind('-');
    if (pos == std::string::npos) return -1;

    std::string suffix = name.substr(pos + 1);
    if (suffix.empty()) return -1;

    for (char c : suffix) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return -1;
    }

    try {
        return std::stoi(suffix);
    } catch (...) {
        return -1;
    }
}

void Hub2::propagateTemperatureByNumber() {
    // Собираем TemperatureSensor по номеру
    std::unordered_map<int, TemperatureSensor*> tempSensors;
    for (auto& dev : devices) {
        auto* ts = dynamic_cast<TemperatureSensor*>(dev.get());
        if (ts) {
            int num = extractNumber(ts->getName());
            if (num >= 0) {
                tempSensors[num] = ts;
            }
        }
    }

    if (tempSensors.empty()) return;

    // Для каждого AirSensor и TrafficLight ищем TemperatureSensor с тем же номером
    for (auto& dev : devices) {
        int num = extractNumber(dev->getName());
        if (num < 0) continue;

        auto it = tempSensors.find(num);
        if (it == tempSensors.end()) continue;

        int temp = it->second->getLastTemperature();

        // AirSensor-N ← TemperatureSensor-N
        if (auto* air = dynamic_cast<AirSensor*>(dev.get())) {
            air->notifyTemperature(temp);
            continue;
        }

        // TrafficLight-N ← TemperatureSensor-N
        if (auto* light = dynamic_cast<TrafficLight*>(dev.get())) {
            light->notifyTemperature(temp);
            continue;
        }
    }
}

void Hub2::addDevice(std::shared_ptr<IoTDevice> device) {
    devices.push_back(device);
}

void Hub2::tick() {
    // Сначала обновляем все устройства
    for (auto& device : devices) {
        device->update();
    }

    // Затем передаём температуру связанным устройствам по номеру
    propagateTemperatureByNumber();

    Logger::info("Шаг завершен");
}
