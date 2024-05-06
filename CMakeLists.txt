cmake_minimum_required(VERSION 3.10)

project(libonvif VERSION 0.1)

if(WIN32)
    add_definitions(-DUSE_WIN32_CRYPTO -D_CRT_SECURE_NO_WARNINGS -DWITH_OPENSSL)
endif(WIN32)

# add_subdirectory(3rdparty/boringssl)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/boringssl/include)

add_subdirectory(3rdparty/openssl-cmake)
add_subdirectory(3rdparty/live555)

include_directories(${OPENSSL_INCLUDE_DIR})

file(GLOB libonvif_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

if(WIN32)
    add_library(libonvif SHARED ${libonvif_SRC})
    target_link_libraries(libonvif ssl crypto)
else()
    add_library(onvif SHARED ${libonvif_SRC})
    target_link_libraries(onvif ssl crypto)
endif()

add_subdirectory(test)
add_subdirectory(live555Rtsp)