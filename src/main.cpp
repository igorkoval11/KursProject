//
// Created by Artemiy on 7.05.26.
//

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
using namespace std;

#include "include/devices/AirSensor.h"
#include "include/core/HUB2.h"
#include "include/devices/IoTDevice.h"
#include "include/core/Power_Grid.h"
#include "include/devices/TrafficLight.h"
#include "include/utils/Logger.h"
#include "include/devices/TemperatureSensors.h"

int main() {
    const int NUM_SENSORS      = 20;
    const int NUM_LIGHTS       = 10;
    const int SIM_STEPS        = 20;
    const int STEP_DELAY_MS    = 500;

    Logger::setLogToConsole(true);
    Logger::setLogToFile(true);

    Hub2 hub;
    PowerGrid grid;
    vector<shared_ptr<IoTDevice>> allDevices;

    auto tempSensor = make_shared<TemperatureSensor>("ТемпДатчик-1");
    hub.addDevice(tempSensor);
    allDevices.push_back(tempSensor);
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

    Logger::info("Запуск симуляции умного города");
    Logger::info("Устройств в сети: " + to_string(allDevices.size()));
    Logger::info("=======================================");

    for (int i = 1; i <= SIM_STEPS; i++) {
        // Начало шага — сбрасываем счётчики событий
        Logger::beginStep(i);
        Logger::info("--- Шаг " + to_string(i) + " ---");

        // Обновляем все устройства через хаб
        // (устройства сами вызывают Logger::info для подробных логов)
        hub.tick();

        // Регистрируем события по типам устройств для сжатой статистики
        for (const auto& dev : allDevices) {
            if (!dev->isOn()) continue;
            // Определяем тип по имени (можно заменить на виртуальный метод)
            const string& n = dev->getName();
            if (n.rfind("Датчик",      0) == 0) Logger::recordEvent("AirSensor");
            else if (n.rfind("Светофор", 0) == 0) Logger::recordEvent("TrafficLight");
            else if (n.rfind("ТемпДатчик",0)==0) Logger::recordEvent("TempSensor");
            else                                   Logger::recordEvent("Other");
        }

        grid.checkLimits(allDevices);

        // Сжатая строка-итог шага + сохранение для финальной таблицы
        Logger::endStep();

        this_thread::sleep_for(chrono::milliseconds(STEP_DELAY_MS));
    }

    Logger::info("=== Симуляция завершена ===");

    // Итоговая таблица по всем шагам
    Logger::printSummary();

    cout << "\nНажмите Enter для выхода...";
    cin.get();
    return 0;
}
