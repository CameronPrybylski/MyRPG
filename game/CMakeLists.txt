cmake_minimum_required(VERSION 3.15)
project(MyRPG)

add_executable(MyRPG
    src/main.cpp
    src/Level.cpp
    src/GameOver.cpp
    src/Player.cpp
    src/PlayerInBattle.cpp
    src/Obstacle.cpp
    src/Enemy.cpp
    src/Sword.cpp
    src/Battle.cpp
    src/BattleMenu.cpp
    # Add your game source files here
)

target_include_directories(MyRPG
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include          #Game Headers   
)

target_link_libraries(MyRPG PRIVATE Engine)

