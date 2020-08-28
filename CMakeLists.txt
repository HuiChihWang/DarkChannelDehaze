cmake_minimum_required(VERSION 3.17)
project(Dehaze)

set(CMAKE_CXX_STANDARD 17)

find_package(opencv REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(Dehaze src/main.cpp src/DarkChannel.cpp src/DarkChannel.h)
target_link_libraries(Dehaze ${OpenCV_LIBS})