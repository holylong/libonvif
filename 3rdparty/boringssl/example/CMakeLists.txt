include_directories(../include)

include_directories(./)
include_directories(../)

if(MSVC)
add_executable(
  exer
  exer.c
 directory_win.c
 directory.h
)
else(UNIX)
add_executable(
  exer
  exer.c
  directory.h
  directory_posix.c
)
endif()

add_definitions(/WX-)

add_dependencies(exer global_target)

if(WIN32)
  target_link_libraries(exer ws2_32)
endif()

if(APPLE OR WIN32 OR ANDROID)
  target_link_libraries(exer ssl crypto)
else()
  find_library(FOUND_LIBRT rt)
  if(FOUND_LIBRT)
    target_link_libraries(exer ssl crypto -lrt)
  else()
    target_link_libraries(exer ssl crypto)
  endif()
endif()
