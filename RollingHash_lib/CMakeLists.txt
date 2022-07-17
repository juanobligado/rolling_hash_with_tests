project(RollingHash)

set(HEADER_FILES
        RsyncFileDiff.h
        RollingChecksum.h CustomTypeDefinitions.h)

set(SOURCE_FILES
        RsyncFileDiff.cpp
        RollingChecksum.cpp
        Sha1.hpp)

add_library(RollingHash_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
