set(BOOST_ROOT "E:\\LiDAR\\boost_1_63_0")
set(BOOST_LIBRARYDIR "E:\\LiDAR\\boost_1_63_0\\stage\\lib")

message("${BOOST_LIBRARYDIR}")

find_package(Boost COMPONENTS iostreams REQUIRED)

if(Boost_FOUND)
    message("${BOOST_LIBRARYDIR} ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    add_library(LiDARCore LasDataset.cpp ErrorCode.h)
    target_link_libraries(LiDARCore ${Boost_LIBRARIES})
endif()