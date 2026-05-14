//
// Created by Artemiy on 2.05.26.
//

#ifndef KURSPROJECT_TRAFFICLIGHT_H
#define KURSPROJECT_TRAFFICLIGHT_H
#include "IoTDevice.h"
#include <string>

class TrafficLight: public IoTDevice{
private:
    bool isRed;
public:
    TrafficLight(std::string name,int power = 20):
        IoTDevice(std::move(name),power)
    {
        isRed=true;
    }

    void update() override;
    void turnOff() override;
    bool isOn() const  override;



};



#endif