cmake_minimum_required(VERSION 3.10)

project(libonvif VERSION 0.1)

add_subdirectory(3rdparty/boringssl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/boringssl/include)
file(GLOB libonvif_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(onvif SHARED ${libonvif_SRC})