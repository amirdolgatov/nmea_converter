cmake_minimum_required(VERSION 3.26)
project(nmea_converter)

# which compilers to use for C and C++
#set(CMAKE_C_COMPILER   /home/amir/Downloads/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
#set(CMAKE_CXX_COMPILER /home/amir/Downloads/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

set(CMAKE_CXX_STANDARD 17)

add_executable(nmea_converter main.cpp)
