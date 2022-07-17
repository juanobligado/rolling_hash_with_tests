project(RollingHash)

set(HEADER_FILES
        RollingHash.h
        RollingChecksum.h CustomTypeDefinitions.h)

set(SOURCE_FILES
        RollingHash.cpp
        RollingChecksum.cpp
        sha1.hpp)

add_library(RollingHash_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
