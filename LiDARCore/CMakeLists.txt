set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

set(SOURCE_FILES LasDataset.cpp LasDataset.h
        ByteStream.cpp ByteStream.h
        ErrorCode.h
        KdTree.cpp KdTree.h
        ExternalSort.cpp ExternalSort.h
        LasWriter.cpp LasWriter.h
        PrivatePointCloudDataset.cpp PrivatePointCloudDataset.h
        LasReader.cpp LasReader.h LasPoint.cpp LasPoint.h)

add_library(LiDARCore ${SOURCE_FILES})
target_link_libraries(LiDARCore ${Boost_LIBRARIES})