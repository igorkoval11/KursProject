//
// Created by matvl on 03.05.2026.
//
#include "core/HUB2.h"
#include "utils/Logger.h"

void Hub2::addDevice(std::shared_ptr<IoTDevice> device) {
    devices.push_back(device);
}

void Hub2::tick() {
    for (auto& device : devices) {
        device->update();
    }
    Logger::info("Шаг завершен");
}
