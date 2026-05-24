//
// Created by Artemiy on 2.05.26.
//
#include "Logger.h"
#include "TrafficLight.h"
using namespace std;

void TrafficLight::update() {
    if (!isOn()) return;

    phaseTimer++;

    const PhaseConfig& currentConfig = schedule[currentPhaseIndex];

    if (phaseTimer >= currentConfig.ticks) {
        phaseTimer = 0;
        currentPhaseIndex = (currentPhaseIndex + 1) % schedule.size();
    }

    const PhaseConfig& newConfig = schedule[currentPhaseIndex];
    Logger::info("[" + name + "] Фаза: " + phaseToString(newConfig.phase) +
                 " (осталось тиков: " + to_string(newConfig.ticks - phaseTimer) + ")");
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
