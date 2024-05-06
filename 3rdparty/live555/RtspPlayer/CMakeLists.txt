cmake_minimum_required(VERSION 3.10)

project(RtspPlayer)

file(GLOB SOURCES "*.cpp" "*.hh" "*.h")

# set(FFMPEG_INCLUDE_DIR "D:/commonlib/include")
# set(FFMPEG_LIBRARY_DIR "D:/commonlib/lib")
# set(OPENCV_INCLUDE_DIR "D:/commonlib/opencv/opencv3416/build/include")
# set(OPENCV_LIBRARY_DIR "D:/commonlib/opencv/opencv3416/build/x64/vc15/lib")

if(WIN32)
include_directories("D:/commonlib/ffmpeg-20200721-b5f1e05-win64-dev/include")
link_directories("D:/commonlib/ffmpeg-20200721-b5f1e05-win64-dev/lib")
set(OpenCV_DIR "D:/commonlib/opencv/opencv455/opencv/build")
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif(MSVC)
endif(WIN32)

include_directories(${CMAKE_SOURCE_DIR}/liveMedia/include)
include_directories(${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include)
include_directories(${CMAKE_SOURCE_DIR}/groupsock/include)
include_directories(${CMAKE_SOURCE_DIR}/UsageEnvironment/include)
include_directories(${FFMPEG_INCLUDE_DIR})
include_directories(${OPENCV_INCLUDE_DIR})


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

target_link_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARY_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${OPENCV_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(${PROJECT_NAME} PRIVATE opencv_world3416d)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE avcodec avutil avdevice swresample swscale avformat)
target_link_libraries(${PROJECT_NAME} PRIVATE liveMedia BasicUsageEnvironment groupsock UsageEnvironment)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)