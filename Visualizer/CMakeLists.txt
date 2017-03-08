cmake_minimum_required(VERSION 3.6)
project(Visualizer)

set(CMAKE_CXX_STANDARD 11)
message("${CMAKE_SOURCE_DIR}/LiDARCore")
include_directories(${CMAKE_SOURCE_DIR}/LiDARCore)

set(SOURCE_FILES main.cpp)

add_executable(Visualizer ${SOURCE_FILES})

find_package(Boost COMPONENTS iostreams REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(Visualizer LiDARCore ${Boost_LIBRARIES})
endif()

add_dependencies(Visualizer LiDARCore)