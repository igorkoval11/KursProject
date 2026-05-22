//
// Created by Artemiy on 1.05.26.
//
#ifndef KURSPROJECT_IOTDEVICE_H
#define KURSPROJECT_IOTDEVICE_H
#include <string>
#include <cmath>

class IoTDevice{
protected:
    bool is_on = true;
    int base_power;
    int power_consumption;
    int energy_consumed = 0;
    std::string name;
    int priority; 
public:
    IoTDevice(std::string name, int power, int priority = 3)
        : name(name), base_power(power), power_consumption(power), priority(priority){}
    virtual ~IoTDevice(){}
    virtual void update()=0;
    virtual void turnOff(){
        is_on = false;
    }
    virtual void turnOn(){
        is_on = true;
    }
    virtual bool isOn() const{
        return is_on;
    }
    virtual bool canBeTurnedOff() const {
        return true;
    }
    int getEnergy() const{
        return energy_consumed;
    }
    int getPowerConsumption() const{
        return is_on ? power_consumption : 0;
    }
    int getBasePower() const{
        return base_power;
    }
    int getPriority() const{
        return priority;
    }
    std::string getName() const{
        return name;
    }
    static double& getCurrentTemperature() {
        static double temp = 20.0;
        return temp;
    }
    void consumeEnergy() {
        if (is_on) {
            double temp = getCurrentTemperature();
            power_consumption = static_cast<int>(base_power * (1.0 + std::abs(temp - 20.0) * 0.012));
            energy_consumed += power_consumption;
        }
    }
};
#endif
