cmake_minimum_required(VERSION 3.5)
project(raycasting)

cmake_policy(SET CMP0074 OLD)
set(SFML_ROOT "C:/Frameworks/SFML-2.5.1")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O3")

set(SOURCE_FILES
        src/main.cpp
        src/game.cpp src/game.h
        src/game_state.h
        src/texture_manager.cpp src/texture_manager.h
        src/player.cpp src/player.h
        src/game_state_run.cpp src/game_state_run.h
        src/map_manager.cpp src/map_manager.h
        src/stripe.h src/stripe.cpp
        src/camera.cpp src/camera.h
        src/fps_counter.cpp src/fps_counter.h
        src/minimap.cpp src/minimap.h
        src/floor_ceiling.cpp src/floor_ceiling.h src/utility/constants.h)
add_executable(raycasting ${SOURCE_FILES})

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(raycasting ${SFML_LIBRARIES})
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)