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

        std::vector<std::shared_ptr<IoTDevice>> toDisconnect;
        for (auto& dev : devices) {
            if (dev->isOn() && !dev->isCritical()) {
                toDisconnect.push_back(dev);
            }
        }

        if (toDisconnect.empty()) {
            Logger::error("[Сеть] Нет устройств для отключения! Работают только критические.");
            return;
        }

        // Сортировка: датчики грязи (priority=3) раньше светофоров (priority=1)
        // Внутри группы — от большего текущего потребления к меньшему
        std::sort(toDisconnect.begin(), toDisconnect.end(),
            [](const std::shared_ptr<IoTDevice>& a, const std::shared_ptr<IoTDevice>& b) {
                if (a->getPriority() != b->getPriority())
                    return a->getPriority() > b->getPriority();
                return a->getPowerConsumption() > b->getPowerConsumption();
            });

        for (auto& dev : toDisconnect) {
            if (total <= limit) break;
            int currentPower = dev->getPowerConsumption();
            dev->turnOff();
            total = 0;
            for (auto& d : devices) {
                if (d->isOn()) total += d->getEnergy();
            }
            Logger::warn("[Сеть] Отключён " + dev->getName() +
                         " (потребление: " + std::to_string(currentPower) + " Вт" +
                         ", приоритет: " + std::to_string(dev->getPriority()) + ")" +
                         ", осталось: " + std::to_string(total) + " Вт");
        }

        if (total > limit) {
            Logger::error("[Сеть] Невозможно устранить перегрузку! Работают только критические устройства.");
        }
    }
}
