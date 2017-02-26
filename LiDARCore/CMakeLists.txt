find_package(Boost COMPONENTS iostreams REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_library(LiDARCore LasDataset.cpp ErrorCode.h)
    target_link_libraries(LiDARCore ${Boost_LIBRARIES})
endif()