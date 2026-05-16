//
// Created by Artemiy on 1.05.26.
//
#ifndef KURSPROJECT_IOTDEVICE_H
#define KURSPROJECT_IOTDEVICE_H
#include <string>
class IoTDevice{
protected:
    bool is_on = true;
    int power_consumption;
    int energy_consumed = 0;
    std::string name;
    int priority; 
public:
    IoTDevice(std::string name, int power, int priority = 3)
        : name(name), power_consumption(power), priority(priority){}
    virtual ~IoTDevice(){}
    virtual void update()=0;
    virtual void turnOff(){
        is_on= false;
    }
    virtual bool isOn() const{
        return is_on;
    }
    int getEnergy() const{
        return energy_consumed;
    }
    int getPriority() const{
        return priority;
    }
    std::string getName() const{
        return name;
    }
    void consumeEnergy() {
        if (is_on) {
            energy_consumed += power_consumption;
        }
    }
};
#endif
