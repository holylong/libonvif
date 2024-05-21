cmake_minimum_required(VERSION 3.10)

project(testonvif VERSION 0.1)

include_directories(${OPENSSL_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/live555/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/live555/groupsock/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/live555/UsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/live555/BasicUsageEnvironment/include)

include_directories(${CMAKE_SOURCE_DIR})


if(WIN32)
    include_directories("D:/commonlib/ffmpeg-20200721-b5f1e05-win64-dev/include")
    link_directories("D:/commonlib/ffmpeg-20200721-b5f1e05-win64-dev/lib")
    set(OpenCV_DIR "D:/commonlib/opencv/opencv455/opencv/build")
    if(MSVC)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    endif(MSVC)
endif(WIN32)

link_directories(${CMAKE_BINARY_DIR}/lib)
link_directories(${CMAKE_BINARY_DIR})

find_package(OpenCV REQUIRED)

file(GLOB live555Rtsp_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.hh)
add_executable(rtspexec ${live555Rtsp_SRC})
target_link_libraries(rtspexec onvif ssl crypto ${OpenCV_LIBS} avcodec avformat avutil swscale avdevice swresample)
target_link_libraries(rtspexec liveMedia groupsock BasicUsageEnvironment UsageEnvironment)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})