project(Tests)

include_directories(${CMAKE_SOURCE_DIR}/LiDARCore)

# find GTest
enable_testing()
find_package(GTest REQUIRED)

# include GTest
include_directories(${GTEST_INCLUDE_DIRS})

# include LASlib
include_directories(../LASTools/LASlib/inc)

add_executable(LasIOTest LasIOTest.cpp KDTreeTest.cpp)

# link GTest
target_link_libraries(LasIOTest ${GTEST_BOTH_LIBRARIES})

# link LiDARCore
target_link_libraries(LasIOTest LiDARCore)

# link laslib
link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/LASTools/LASlib)
target_link_libraries(LasIOTest laslib)

add_dependencies(LasIOTest LiDARCore)

add_test(AllTests LasIOTest)