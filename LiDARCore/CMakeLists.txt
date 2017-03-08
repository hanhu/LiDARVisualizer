set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -Wno-deprecated-declarations)

find_package(Boost COMPONENTS iostreams REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_library(LiDARCore LasDataset.cpp LasDataset.h ByteStream.cpp ByteStream.h ErrorCode.h KdTree.cpp KdTree.h ExternalSort.cpp ExternalSort.h LasWriter.cpp LasWriter.h PrivatePointCloudDataset.cpp PrivatePointCloudDataset.h LasReader.cpp LasReader.h)
    target_link_libraries(LiDARCore ${Boost_LIBRARIES})
endif()