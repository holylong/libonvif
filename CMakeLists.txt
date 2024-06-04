cmake_minimum_required(VERSION 3.10)

project(libonvif VERSION 0.1)

if(WIN32)
    add_definitions(-DUSE_WIN32_CRYPTO -D_CRT_SECURE_NO_WARNINGS -DWITH_OPENSSL)
elseif(UNIX)
    add_definitions(-DWITH_OPENSSL)
endif(WIN32)

set(BUILD_SHARED_LIBS ON)

# add_subdirectory(3rdparty/boringssl)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/boringssl/include)

add_subdirectory(3rdparty/openssl-cmake)
add_subdirectory(3rdparty/live555)

# add_definitions(-DWITH_C_LOCALE)
if(UNIX)
    add_definitions(-DXLOCALE_NOT_USED)
    add_definitions(-DUNIX)
endif()

# 在你的CMakeLists.txt的适当位置添加以下命令
add_compile_options(-Wint-to-pointer-cast)

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

find_package(Qt5 COMPONENTS Core Widgets)
find_package(Qt6 COMPONENTS Core Widgets)
if(Qt5_FOUND OR Qt6_FOUND)
add_subdirectory(${CMAKE_SOURCE_DIR}/qcamsuit)
endif()
