cmake_minimum_required(VERSION 3.2)
project(punching-bag)
include(ExternalProject)

ExternalProject_Add(RakNet
  GIT_REPOSITORY https://github.com/ltjax/RakNet.git
  CMAKE_ARGS
    -DRAKNET_ENABLE_DLL=0
    -DRAKNET_ENABLE_SAMPLES=0
    -DRAKNET_ENABLE_STATIC=1
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/raknet/
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
  UPDATE_COMMAND ""
)

set(MAIN_TARGET punching-bag)
add_executable(${MAIN_TARGET} main.cpp)


target_include_directories(${MAIN_TARGET}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/raknet/include/)

target_link_libraries(${MAIN_TARGET}
    ${CMAKE_CURRENT_BINARY_DIR}/raknet/lib/libRakNetLibStatic.a
    pthread)

set_property(TARGET ${MAIN_TARGET} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${MAIN_TARGET} PROPERTY CXX_STANDARD_REQUIRED ON)
