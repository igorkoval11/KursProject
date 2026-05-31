//
// Created by matvl on 03.05.2026.
//

#ifndef KURSPROJECT_HUB2_H
#define KURSPROJECT_HUB2_H

#include <vector>
#include <memory>
#include "IoTDevice.h"

class Hub2 {
private:
    std::vector<std::shared_ptr<IoTDevice>> devices;

    // Извлекает числовой суффикс из имени вида "Префикс-N"
    // Возвращает -1, если суффикс не найден
    static int extractNumber(const std::string& name);

    // После обновления всех устройств передаёт температуру
    // от каждого TemperatureSensor-N к AirSensor-N и TrafficLight-N
    void propagateTemperatureByNumber();

public:
    void addDevice(std::shared_ptr<IoTDevice> device);
    void tick();
};

#endif 