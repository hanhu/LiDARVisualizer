include_directories(${CMAKE_SOURCE_DIR}/LiDARCore)

set(SOURCE_FILES main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework GLUT")

#find_package(OpenGL REQUIRED)
#find_package(GLM REQUIRED)
#find_package(GLEW REQUIRED)
find_package(glfw3 3.2 REQUIRED)

add_executable(Visualizer ${SOURCE_FILES})
target_link_libraries(Visualizer LiDARCore ${Boost_LIBRARIES}  glfw)

add_dependencies(Visualizer LiDARCore)