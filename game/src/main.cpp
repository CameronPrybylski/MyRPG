#include <Engine/Engine.h>
#include <Engine/Scene/Application.h>
#include <Game/Level.h>
#include <Game/Battle.h>
#include <Game/GameOver.h>
#include <Game/Player.h>
#include <Game/Obstacle.h>
#include <iostream>

int main()
{
    
    Application app;
    
    if(!app.Create("Game", 1067, 800)){
        //1067 / 600 = 1.7783333
        std::cout << "Error Creating Engine" << std::endl;
    }
    else{
    
        auto quadMesh = std::make_shared<QuadMesh>();
        AssetManager::LoadMesh("quadMesh", quadMesh);
        
        std::shared_ptr<Scene> overworld = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/overworld.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveOverWorld.json");
        std::shared_ptr<Scene> battle = std::make_shared<Battle>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/battle.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json");
        //std::shared_ptr<Scene> gameOver = std::make_shared<GameOver>(app.GetScreenWidth(), app.GetScreenHeight());

        app.AddScene("overworld", overworld);
        app.AddScene("battle", battle);
        //app.AddScene("gameOver", gameOver);
        app.SetScene("overworld");
        
        AssetManager::LoadShader("objectShader", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/basic.vert", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/basic.frag");
        AssetManager::LoadShader("textureShader", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/texture.vert", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/texture.frag");
        app.Run();
    }
    return 0;
}