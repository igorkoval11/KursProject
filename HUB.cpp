//
// Created by matvl on 02.05.2026.
//

#include "Hub.h"
#include <iostream>

void Hub::addDevice(std::shared_ptr<IoTDevice> device) {
    devices.push_back(device);
}

void Hub::tick() {
    for (auto& device : devices) {
        device->update();
    }
    std::cout << "Шаг завершен" << std::endl;
}