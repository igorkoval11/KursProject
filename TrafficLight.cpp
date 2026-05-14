//
// Created by Artemiy on 2.05.26.
//
#include "Logger.h"
#include "TrafficLight.h"
using namespace std;

void TrafficLight::update() {
    isRed = !isRed;
    Logger::info("[" + name + "] " + (isRed ? "Красный" : "Зеленый"));
    consumeEnergy();
}

void TrafficLight::turnOff() {
    is_on = false;
}

bool TrafficLight::isOn() const {
    return is_on;
}
