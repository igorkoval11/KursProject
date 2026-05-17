//
// Created by matvl on 05.05.2026.
//
#ifndef KURSPROJECT_POWERGRID_H
#define KURSPROJECT_POWERGRID_H

#include <vector>
#include <memory>
#include "IoTDevice.h"

class PowerGrid {
private:
    int limit;

public:
    explicit PowerGrid(int powerLimit=500): limit(powerLimit){}
    void checkLimits(const std::vector<std::shared_ptr<IoTDevice>>& devices);
};

#endif 