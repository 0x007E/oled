cmake_minimum_required(VERSION 3.21)

project(avr_firmwares C)

include(${CMAKE_SOURCE_DIR}/cmake/platforms/avr/usage-avr-size.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/platforms/avr/build-avr-firmware.cmake)

add_subdirectory(avr/frame)
add_subdirectory(avr/tty)
add_subdirectory(avr0/frame)
add_subdirectory(avr0/tty)