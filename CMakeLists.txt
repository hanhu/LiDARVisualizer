cmake_minimum_required(VERSION 3.5.2)
project(LiDARVisualizer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -m64")

# print runtime output directory
#message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

find_package(Boost COMPONENTS iostreams REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

add_subdirectory(LiDARCore)
add_subdirectory(Visualizer)
add_subdirectory(Tests)

# add LASTools
add_subdirectory(LASTools)