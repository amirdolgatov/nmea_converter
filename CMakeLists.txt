cmake_minimum_required(VERSION 3.26)
project(nmea_converter)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(BBB-UART-anybaud)
add_executable(nmea_converter main.cpp)
target_link_libraries(nmea_converter PUBLIC bbb_uart)
target_include_directories(nmea_converter PUBLIC BBB-UART-anybaud)