set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

set(SOURCE_FILES LasDataset.cpp LasDataset.h
        ByteStream.cpp ByteStream.h
        ErrorCode.h
        LasWriter.cpp LasWriter.h
        KdTreeDatasetWriter.h
        LasReader.cpp LasReader.h
        LasPoint.cpp LasPoint.h
        MemoryMappedFile.cpp MemoryMappedFile.h
        Iterator.cpp Iterator.h
        ConstIterator.cpp ConstIterator.h
        ConstContainerBase.cpp ConstContainerBase.h
        KdTreeDatasetWriter.cpp KdTreeDatasetWriter.h
        Vector.cpp Vector.h
        OutMemoryMappedFile.cpp OutMemoryMappedFile.h
        InMemoryMappedFile.cpp InMemoryMappedFile.h
        NonConstContainerBase.cpp NonConstContainerBase.h
        ContainerBase.cpp ContainerBase.h)

add_library(LiDARCore ${SOURCE_FILES})
target_link_libraries(LiDARCore ${Boost_LIBRARIES})