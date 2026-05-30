//
// Created by matvl on 04.05.2026.
//
#include "AirSensor.h"
#include "Logger.h"
#include <random>

void AirSensor::update() {
    if (!isOn()) return;

    // Логика связи: датчик грязи активен, когда связанный светофор горит КРАСНЫМ
    // (машины стоят — загрязнение воздуха максимально)
    auto light = linkedLight.lock();
    if (light) {
        if (light->getCurrentPhase() == TrafficLight::Phase::RED) {
            // Красный — замеряем
            Logger::info("[" + name + "] Светофор " + light->getName() +
                         " красный — измерение активно");
        } else {
            // Не красный — пропускаем замер в этот тик
            Logger::info("[" + name + "] Светофор " + light->getName() +
                         " не красный (" + light->phaseToString(light->getCurrentPhase()) +
                         ") — датчик ожидает");
            return;
        }
    }

    // Потребление фиксированное
    power_consumption = base_power;
    consumeEnergy();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    int level = dis(gen);
    Logger::info("[" + name + "] Уровень загрязнения: " + std::to_string(level) +
                 "% | Потребление: " + std::to_string(power_consumption) + " Вт (фикс.)");

    if (level > alarmThreshold) {
        Logger::warn("[" + name + "] ТРЕВОГА — превышен порог загрязнения (" +
                     std::to_string(level) + "% > " + std::to_string(alarmThreshold) + "%)");
    }
}
