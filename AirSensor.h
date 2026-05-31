//
// Created by matvl on 04.05.2026.
//

#ifndef KURSPROJECT_AIRSENSOR_H
#define KURSPROJECT_AIRSENSOR_H

#include "IoTDevice.h"

class AirSensor : public IoTDevice {
    int alarmThreshold;

public:
    explicit AirSensor(std::string name, int threshold = 70, int power = 15)
        : IoTDevice(std::move(name), power, 3), // приоритет 3 — некритический
          alarmThreshold(threshold) {}

    void update() override;
};

#endif