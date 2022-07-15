project(RollingHash)

set(HEADER_FILES
        RollingHash.h
        )

set(SOURCE_FILES
        RollingHash.cpp
        )

add_library(RollingHash_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})