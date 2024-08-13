cmake_minimum_required(VERSION 3.10)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(ByteHamsterUtil
        DESCRIPTION "Various basic data structures"
        HOMEPAGE_URL "https://github.com/ByteHamster/Util"
        VERSION 1.0
        LANGUAGES CXX)

if(TARGET ByteHamsterUtil)
    return()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_compile_options(-march=native)
endif()

add_library(ByteHamsterUtil INTERFACE)
target_include_directories(ByteHamsterUtil INTERFACE include)
target_compile_options(ByteHamsterUtil INTERFACE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wpedantic>)
target_compile_features(ByteHamsterUtil INTERFACE cxx_std_20)

add_subdirectory(extern/sdsl-lite/external EXCLUDE_FROM_ALL)
add_subdirectory(extern/sdsl-lite/include EXCLUDE_FROM_ALL)
add_subdirectory(extern/sdsl-lite/lib EXCLUDE_FROM_ALL)
target_compile_options(sdsl PRIVATE -w)
target_include_directories(ByteHamsterUtil SYSTEM INTERFACE extern/sdsl-lite/include)
target_link_libraries(ByteHamsterUtil INTERFACE sdsl)

set(TLX_INSTALL_INCLUDE_DIR tlx CACHE PATH "Workaround for TLX breaking the first cmake call")
set(IPS2RA_DISABLE_PARALLEL ON CACHE PATH "FindTBB greps a file that does not exist in recent TBB versions")
add_subdirectory(extern/pasta-bit-vector EXCLUDE_FROM_ALL)
target_compile_options(tlx PRIVATE -w)
target_link_libraries(ByteHamsterUtil INTERFACE pasta_bit_vector)
target_include_directories(ByteHamsterUtil SYSTEM INTERFACE extern/pasta-bit-vector)
