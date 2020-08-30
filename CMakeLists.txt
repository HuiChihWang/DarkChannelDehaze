cmake_minimum_required(VERSION 3.17)
project(Dehaze)

set(CMAKE_CXX_STANDARD 17)

find_package(opencv REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(Dehaze src/main.cpp src/dark_channel.cpp src/dark_channel.h include/image_dehaze.hpp src/dark_channel_dehazer.cpp src/dark_channel_dehazer.h include/parameter_struct.h)
target_link_libraries(Dehaze ${OpenCV_LIBS})