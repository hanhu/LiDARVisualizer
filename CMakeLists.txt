cmake_minimum_required(VERSION 3.6)
project(LiDARVisualizer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Visualizer/main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(LiDARCore)

add_executable(Visualizer ${SOURCE_FILES})
target_link_libraries(Visualizer LiDARCore)