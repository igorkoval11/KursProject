//
// Created by Artemiy on 2.05.26.
//

#ifndef KURSPROJECT_TRAFFICLIGHT_H
#define KURSPROJECT_TRAFFICLIGHT_H

#include "IoTDevice.h"
#include <string>
#include <random>

class TrafficLight : public IoTDevice {
public:
    enum class Phase { RED, YELLOW, GREEN };

private:
    Phase currentPhase;
    int phaseTimer;
    int redDuration;
    int yellowDuration;
    int greenDuration;

public:
    TrafficLight(std::string name, int power = 20,
                 int redDur = 2, int yellowDur = 1, int greenDur = 2)
        : IoTDevice(std::move(name), power, 1),
          phaseTimer(0),
          redDuration(redDur),
          yellowDuration(yellowDur),
          greenDuration(greenDur)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 2);
        currentPhase = static_cast<Phase>(dis(gen));
    }

    void update() override;
    void turnOff() override;
    bool isOn() const override;

private:
    std::string phaseToString(Phase phase) const;
};

#endif
