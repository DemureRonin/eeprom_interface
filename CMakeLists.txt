cmake_minimum_required(VERSION 4.0)
project(eeprom)

set(CMAKE_CXX_STANDARD 20)

add_executable(eeprom
        spi_interface.h
        eeprom.cpp

        eeprom_constants.h
        eeprom.h
        )
