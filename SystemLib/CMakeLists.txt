cmake_minimum_required(VERSION 3.20)

project(SystemLib)

set(SYSTEM_LIB_SOURCES main.c eeprom.c ram.c bus.c W65C02.c cycles.c vrEmuLcd.c system_types.h W65C22.c W65C22.h system.h system.c)

include_directories(${CMAKE_SOURCE_DIR}/CSFML/include)

add_executable(main ${SYSTEM_LIB_SOURCES})

target_link_libraries(main ${CMAKE_SOURCE_DIR}/CSFML/lib/libsfml-system.a ${CMAKE_SOURCE_DIR}/CSFML/lib/libsfml-window.a ${CMAKE_SOURCE_DIR}/CSFML/lib/libsfml-graphics.a)