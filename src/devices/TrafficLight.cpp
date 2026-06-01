//
// Created by Artemiy on 2.05.26.
//
#include "utils/Logger.h"
#include "devices/TrafficLight.h"
using namespace std;

void TrafficLight::update() {
    if (!isOn()) return;

    phaseTimer++;

    const PhaseConfig& currentConfig = schedule[currentPhaseIndex];

    if (phaseTimer >= currentConfig.ticks) {
        phaseTimer = 0;
        currentPhaseIndex = (currentPhaseIndex + 1) % schedule.size();
    }

    Logger::info("[" + name + "] Фаза: " + phaseToString(getCurrentPhase()) +
                 " (осталось тиков: " + to_string(schedule[currentPhaseIndex].ticks - phaseTimer) + ")");
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
