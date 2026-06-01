cmake_minimum_required(VERSION 3.14)
project(KursProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(KursProject
        include/devices/IoTDevice.h
        src/main.cpp
        src/core/HUB2.cpp
        include/core/HUB2.h
        src/devices/AirSensor.cpp
        include/devices/AirSensor.h
        src/devices/TrafficLight.cpp
        include/devices/TrafficLight.h
        src/core/Power_Grid.cpp
        include/core/Power_Grid.h
        src/devices/TemperatureSensors.cpp
        include/devices/TemperatureSensors.h
        src/utils/Logger.cpp
        include/utils/Logger.h

)
