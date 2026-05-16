//
// Created by matvl on 05.05.2026.
//
#include "Power_Grid.h"
#include "Logger.h"
#include <algorithm>

void PowerGrid::checkLimits(const std::vector<std::shared_ptr<IoTDevice>>& devices) {
    int total = 0;
    for (auto& dev : devices) {
        if (dev->isOn()) total += dev->getEnergy();
    }

    Logger::info("[Сеть] Суммарное потребление: " + std::to_string(total) + " Вт");

    if (total > limit) {
        Logger::error("[Сеть] ПЕРЕГРУЗКА! Отключаем устройства...");

        
        std::vector<std::shared_ptr<IoTDevice>> sortedDevices;
        for (auto& dev : devices) {
            if (dev->isOn()) {
                sortedDevices.push_back(dev);
            }
        }

        /
        std::sort(sortedDevices.begin(), sortedDevices.end(),
            [](const std::shared_ptr<IoTDevice>& a, const std::shared_ptr<IoTDevice>& b) {
                return a->getPriority() > b->getPriority(); 
            });

        
        for (auto& dev : sortedDevices) {
            if (total <= limit) break;
            int powerBefore = dev->getEnergy();
            dev->turnOff();
            total -= dev->getEnergy();
            Logger::warn("[Сеть] Отключён " + dev->getName() +
                         " (потреблял: " + std::to_string(powerBefore) + " Вт" +
                         ", приоритет: " + std::to_string(dev->getPriority()) + ")" +
                         ", осталось: " + std::to_string(total) + " Вт");
        }
    }
}
