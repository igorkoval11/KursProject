//
// Created by matvl on 05.05.2026.
//

#include "Power_Grid.h"
#include "Logger.h"

void PowerGrid::checkLimits(const std::vector<std::shared_ptr<IoTDevice>>& devices) {
    int total = 0;
    for (auto& dev : devices) {
        if (dev->isOn()) total += dev->getEnergy();
    }

    Logger::info("[Сеть] Суммарное потребление: " + std::to_string(total) + " Вт");

    if (total > limit) {
        Logger::error("[Сеть] ПЕРЕГРУЗКА! Отключаем устройства...");
        for (auto& dev : devices) {
            if (dev->isOn() && total > limit) {
                dev->turnOff();
                total -= dev->getEnergy();
                Logger::warn("[Сеть] Устройство отключено, осталось: " + std::to_string(total) + " Вт");
            }
        }
    }
}
