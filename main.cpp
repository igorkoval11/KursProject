//
// Created by Artemiy on 7.05.26.
//
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
using namespace std;

#include "AirSensor.h"
#include "HUB2.h"
#include "IoTDevice.h"
#include "Power_Grid.h"
#include "TrafficLight.h"
#include "Logger.h"
#include "TemperatureSensors.h"


int main() {
    const int NUM_SENSORS = 20;
    const int NUM_LIGHTS = 10;
    const int NUM_TEMP_SENSORS = 5;
    const int SIM_STEPS = 20;
    const int STEP_DELAY_MS = 500;

    Logger::setLogToConsole(true);
    Logger::setLogToFile(true);

    Hub2 hub;
    PowerGrid grid;

    vector<shared_ptr<IoTDevice>> allDevices;

    for (int i = 1; i <= NUM_SENSORS; i++) {
        auto sensor = make_shared<AirSensor>("Датчик-" + to_string(i));
        hub.addDevice(sensor);
        allDevices.push_back(sensor);
    }

    for (int i = 1; i <= NUM_LIGHTS; i++) {
        auto light = make_shared<TrafficLight>("Светофор-" + to_string(i));
        hub.addDevice(light);
        allDevices.push_back(light);
    }

    for (int i = 1; i <= NUM_TEMP_SENSORS; i++) {
        auto tempSensor = make_shared<TemperatureSensor>(
            "ТемпДатчик-" + to_string(i));
        hub.addDevice(tempSensor);
        allDevices.push_back(tempSensor);
    }

    Logger::info("Запуск симуляции умного города");
    Logger::info("Устройств в сети: " + to_string(allDevices.size()));
    Logger::info("=======================================");

    for (int i = 1; i <= SIM_STEPS; i++) {
        Logger::info("--- Шаг " + to_string(i) + " ---");

        hub.tick();
        grid.checkLimits(allDevices);

        this_thread::sleep_for(chrono::milliseconds(STEP_DELAY_MS));
    }

    Logger::info("=== Симуляция завершена ===");
    
    cout << "\nНажмите Enter для выхода...";
    cin.get();
    
    return 0;
}
