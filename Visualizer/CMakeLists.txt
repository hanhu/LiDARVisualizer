include_directories(${CMAKE_SOURCE_DIR}/LiDARCore)

set(SOURCE_FILES main.cpp)

add_executable(Visualizer ${SOURCE_FILES})
target_link_libraries(Visualizer LiDARCore ${Boost_LIBRARIES})

add_dependencies(Visualizer LiDARCore)