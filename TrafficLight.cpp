//
// Created by Artemiy on 2.05.26.
//

#include "Logger.h"
#include "TrafficLight.h"

using namespace std;

void TrafficLight::update() {
    if (!isOn()) return;

    phaseTimer++;

    int maxTicks = 0;
    switch (currentPhase) {
        case Phase::RED:    maxTicks = redDuration;    break;
        case Phase::YELLOW: maxTicks = yellowDuration; break;
        case Phase::GREEN:  maxTicks = greenDuration;  break;
    }

    if (phaseTimer >= maxTicks) {
        phaseTimer = 0;
        switch (currentPhase) {
            case Phase::RED:    currentPhase = Phase::GREEN;  break;
            case Phase::GREEN:  currentPhase = Phase::YELLOW; break;
            case Phase::YELLOW: currentPhase = Phase::RED;    break;
        }
    }

    Logger::info("[" + name + "] Фаза: " + phaseToString(currentPhase));
    consumeEnergy();
}

void TrafficLight::turnOff() {
    is_on = false;
}

bool TrafficLight::isOn() const {
    return is_on;
}

string TrafficLight::phaseToString(Phase phase) const {
    switch (phase) {
        case Phase::RED:    return "КРАСНЫЙ";
        case Phase::YELLOW: return "ЖЁЛТЫЙ";
        case Phase::GREEN:  return "ЗЕЛЁНЫЙ";
    }
    return "НЕИЗВЕСТНО";
}
