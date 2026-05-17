cmake_minimum_required(VERSION 3.14)
project(KursProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(KursProject
        IoTDevice.h
        main.cpp
        HUB2.cpp
        HUB2.h
        AirSensor.cpp
        AirSensor.h
        TrafficLight.cpp
        TrafficLight.h
        Power_Grid.cpp
        Power_Grid.h
        TemperatureSensors.cpp
        TemperatureSensors.h
        Logger.cpp
        Logger.h
        )
