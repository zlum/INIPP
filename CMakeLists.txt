cmake_minimum_required(VERSION 2.8)
project(inipp LANGUAGES CXX)

option(IPP_TEST_BUILD "Build test binary" OFF)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_CXX_STANDARD 14)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}\
        -std=gnu++14 -Wall -Wextra -Wpedantic")
endif(NOT MSVC)

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif(NOT WIN32)

set(INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(IPP_INCLUDE
    ${INCLUDE}
    PARENT_SCOPE
)

include_directories(${INCLUDE})

set(IPP_TEST
    test/test.cpp
)

set(IPP_SRC
    include/inipp.hpp
    src/core/ipputility.hpp
    src/core/ippcore.cpp
    src/core/ippcore.hpp
    src/file/chainlock.cpp
    src/file/chainlock.h
    src/file/file.cpp
    src/file/file.h
    src/file/ippreader.cpp
    src/file/ippreader.h
    src/streams/ipptuple.hpp
    src/streams/streams.hpp
    src/types/ippcache.cpp
    src/types/ippcache.h
    src/types/ippsection.cpp
    src/types/ippsection.h
    src/types/ipptypes.cpp
    src/types/ipptypes.hpp
)

if(IPP_TEST_BUILD)
    set(SOURCE
        ${IPP_TEST}
        ${IPP_SRC}
    )

    add_compile_options(-DIPP_TEST_BUILD)
    add_executable(${PROJECT_NAME} ${SOURCE})
else(IPP_TEST_BUILD)
    set(SOURCE
        ${IPP_SRC}
    )

    add_library(${PROJECT_NAME} ${SOURCE})
endif(IPP_TEST_BUILD)
