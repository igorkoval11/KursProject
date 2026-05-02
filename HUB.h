//
// Created by matvl on 02.05.2026.
//

#ifndef KURSPROJECT_HUB_H
#define KURSPROJECT_HUB_H

#include <vector>
#include <memory>
#include "IoTDevice.h"

class Hub {
private:
    std::vector<std::shared_ptr<IoTDevice>> devices;

public:
    void addDevice(std::shared_ptr<IoTDevice> device);
    void tick();
};

#endif //KURSPROJECT_HUB_H //KURSPROJECT_HUB_H