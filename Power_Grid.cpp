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

    // --- ОТКЛЮЧЕНИЕ при перегрузке ---
    if (total > limit) {
        Logger::error("[Сеть] ПЕРЕГРУЗКА! Отключаем устройства...");

        std::vector<std::shared_ptr<IoTDevice>> toDisconnect;
        for (auto& dev : devices) {
            if (dev->isOn() && dev->canBeTurnedOff()) {
                toDisconnect.push_back(dev);
            }
        }

        if (toDisconnect.empty()) {
            Logger::error("[Сеть] Нет устройств для отключения! Работают только критические.");
        } else {
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
                Logger::error("[Сеть] Невозможно устранить перегрузку!");
            }
        }
    }

    // --- ВКЛЮЧЕНИЕ устройств при наличии резерва ---
    if (total < limit) {
        std::vector<std::shared_ptr<IoTDevice>> offDevices;
        for (auto& dev : devices) {
            if (!dev->isOn()) {
                offDevices.push_back(dev);
            }
        }

        // Сортировка: температура(2) -> светофоры(1) -> грязь(3)
        std::sort(offDevices.begin(), offDevices.end(),
            [](const std::shared_ptr<IoTDevice>& a, const std::shared_ptr<IoTDevice>& b) {
                auto order = [](int p) { return p == 2 ? 0 : (p == 1 ? 1 : 2); };
                return order(a->getPriority()) < order(b->getPriority());
            });

        for (auto& dev : offDevices) {
            // Оценка потребления через base_power с учётом температуры
            double temp = IoTDevice::getCurrentTemperature();
            int estimatedPower = static_cast<int>(dev->getBasePower() * (1.0 + std::abs(temp - 20.0) * 0.012));

            if (total + estimatedPower <= limit) {
                dev->turnOn();
                total += estimatedPower;
                Logger::info("[Сеть] Включён " + dev->getName() +
                             " (потребление: ~" + std::to_string(estimatedPower) + " Вт" +
                             ", приоритет: " + std::to_string(dev->getPriority()) + ")" +
                             ", резерв: " + std::to_string(limit - total) + " Вт");
            }
        }
    }

    // --- РЕЗЕРВ ---
    int reserve = limit - total;
    int reservePercent = (limit > 0) ? (reserve * 100 / limit) : 0;
    int lightPower = 20;
    int sensorPower = 15;
    int possibleLights = (reserve > 0) ? reserve / lightPower : 0;
    int possibleSensors = (reserve > 0) ? reserve / sensorPower : 0;

    Logger::info("[Сеть] Резерв: " + std::to_string(reserve) + " Вт (" +
                 std::to_string(reservePercent) + "%) | Можно подключить: " +
                 std::to_string(possibleLights) + " светофоров и " +
                 std::to_string(possibleSensors) + " датчиков грязи");
}
