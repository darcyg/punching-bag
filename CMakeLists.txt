cmake_minimum_required(VERSION 3.2)
project(punching-bag)
include(ExternalProject)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -static-libstdc++)

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

ExternalProject_Add(Cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/cxxopts/
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
  UPDATE_COMMAND ""
)

set(MAIN_TARGET punching-bag)
add_executable(${MAIN_TARGET} main.cpp)


target_include_directories(${MAIN_TARGET}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/cxxopts/include/
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/raknet/include/)

target_link_libraries(${MAIN_TARGET}
    ${CMAKE_CURRENT_BINARY_DIR}/raknet/lib/libRakNetLibStatic.a
    pthread)

set_property(TARGET ${MAIN_TARGET} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${MAIN_TARGET} PROPERTY CXX_STANDARD_REQUIRED ON)


install(TARGETS ${MAIN_TARGET}
    RUNTIME DESTINATION ./)

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_NAME ${MAIN_TARGET})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Marius Elvert")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
