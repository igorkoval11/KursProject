//
// Created by matvl on 04.05.2026.
//
#ifndef KURSPROJECT_AIRSENSOR_H
#define KURSPROJECT_AIRSENSOR_H

#include "IoTDevice.h"
#include "TrafficLight.h"
#include <memory>

class AirSensor : public IoTDevice {
    int alarmThreshold;
    std::weak_ptr<TrafficLight> linkedLight; // связанный светофор с тем же номером

public:
    explicit AirSensor(std::string name, int threshold = 70, int power = 15)
        : IoTDevice(std::move(name), power, 3),
          alarmThreshold(threshold) {}

    // Привязать светофор с тем же номером
    void linkTrafficLight(std::shared_ptr<TrafficLight> light) {
        linkedLight = light;
    }

    void update() override;

    // Потребление фиксированное — не зависит от температуры
    void refreshPowerConsumption() {
        power_consumption = base_power;
    }
};

#endif
