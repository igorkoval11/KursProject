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

public:
    void addDevice(std::shared_ptr<IoTDevice> device);
    void tick();
};

#endif 