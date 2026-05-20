#ifndef KURSPROJECT_TEMPERATURESENSORS_H
#define KURSPROJECT_TEMPERATURESENSORS_H
#include "IoTDevice.h"

class TemperatureSensor : public IoTDevice {
    static constexpr int BASE_POWER = 10;

public:
    explicit TemperatureSensor(std::string name)
        : IoTDevice(std::move(name), BASE_POWER, 2, true) {}
    void update() override;
};
#endif
