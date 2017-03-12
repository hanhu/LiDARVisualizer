file(GLOB_RECURSE LASLIB_SOURCE_FILES *.hpp *.cpp)

add_definitions(-DLASZIPDLL_EXPORTS)

include_directories(inc)
add_library(laslib ${LASLIB_SOURCE_FILES})

# to successfully build, the #ifdef LASZIPDLL_EXPORTS needs to be deleted throughout the code