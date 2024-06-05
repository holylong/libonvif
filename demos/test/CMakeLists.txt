cmake_minimum_required(VERSION 3.10)

project(testonvif VERSION 0.1)

include_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/openssl-cmake/include)
include_directories(${CMAKE_BINARY_DIR}/3rdparty/openssl-cmake/include)

if(WIN32)
include_directories("${FFMPEG_DIR}/include")
link_directories("${FFMPEG_DIR}/lib")
# set(OpenCV_DIR "D:/commonlib/opencv/opencv455/opencv/build")
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif(MSVC)
endif(WIN32)

find_package(OpenCV REQUIRED)


file(GLOB testonvif_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_executable(testonvif ${testonvif_SRC})
if(WIN32)
    target_link_libraries(testonvif libonvif ssl crypto ${OpenCV_LIBS} avcodec avformat avutil swscale avdevice swresample)
else()
    target_link_libraries(testonvif onvif ssl crypto ${OpenCV_LIBS} avcodec avformat avutil swscale avdevice swresample)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if (WIN32)
    if(CMAKE_BUILD_TYPE MATCHES "Release")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_OpenCV_LIB_PATH}/opencv_world455.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_OpenCV_LIB_PATH}/opencv_videoio_ffmpeg455_64.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_OpenCV_LIB_PATH}/opencv_videoio_msmf455_64.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    else()
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_OpenCV_LIB_PATH}/opencv_world455d.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_OpenCV_LIB_PATH}/opencv_videoio_ffmpeg455_64.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_OpenCV_LIB_PATH}/opencv_videoio_msmf455_64d.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()
endif ()