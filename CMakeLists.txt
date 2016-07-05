cmake_minimum_required(VERSION 3.5)
project(raycasting)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        main.cpp
        game.cpp game.h
        game_state.h
        texture_manager.cpp texture_manager.h
        player.cpp player.h
        game_state_run.cpp game_state_run.h map_manager.cpp map_manager.h)
add_executable(raycasting ${SOURCE_FILES})

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(raycasting ${SFML_LIBRARIES})
endif()