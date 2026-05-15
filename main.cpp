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


int main() {
    const int NUM_SENSORS = 20;
    const int NUM_LIGHTS = 10;
    const int SIM_STEPS = 20;
    const int STEP_DELAY_MS = 500;
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

    cout << " Запуск симуляции умного города " << endl;
    cout << "Устройств в сети: " << allDevices.size() << endl;
    cout << "=======================================" << endl << endl;

    for (int i = 1; i <= SIM_STEPS; i++) {
        cout << "--- Шаг " << i << " ---" << endl;

        // 1. Обновляем все устройства через хаб
        hub.tick();

        // 2. Проверяем электросеть
        grid.checkLimits(allDevices);

        cout << endl;

        this_thread::sleep_for(chrono::milliseconds(STEP_DELAY_MS));
    }

    cout << "=== Симуляция завершена ===" << endl;
    return 0;
}
