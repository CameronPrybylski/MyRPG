cmake_minimum_required(VERSION 3.15)
project(MyRPG)

add_executable(MyRPG
    src/main.cpp
    src/OverWorld/Level.cpp
    src/GameOver.cpp
    src/Menu.cpp
    src/OverWorld/Player.cpp
    src/OverWorld/SaveSpot.cpp
    src/OverWorld/TownSpot.cpp
    src/Battle/PlayerInBattle.cpp
    src/Obstacle.cpp
    src/OverWorld/Enemy.cpp
    src/OverWorld/NPC.cpp
    src/OverWorld/DialogueBox.cpp
    src/Sword.cpp
    src/Battle/Battle.cpp
    src/Battle/BattleMenu.cpp
    src/Battle/MenuItem.cpp
    src/Battle/EnemyInBattle.cpp
    src/Battle/Weapon.cpp
    src/Title/TitleScene.cpp
    src/Title/TitleMenu.cpp
    src/Title/SaveScene.cpp
    src/Title/SaveMenu.cpp
    # Add your game source files here
)

target_include_directories(MyRPG
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include          #Game Headers   
)

target_link_libraries(MyRPG PRIVATE Engine)

###cmake -DCMAKE_BUILD_TYPE=Debug .

