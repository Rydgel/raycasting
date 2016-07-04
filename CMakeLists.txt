cmake_minimum_required(VERSION 3.5)
project(raycasting)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp player.cpp player.h)
add_executable(raycasting ${SOURCE_FILES})

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(raycasting ${SFML_LIBRARIES})
endif()