#include <Engine/Engine.h>
#include <Engine/Scene/Application.h>
#include <Game/Level.h>
#include <Game/TitleScene.h>
#include <Game/SaveScene.h>
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
        
        std::shared_ptr<Scene> title = std::make_shared<TitleScene>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/title.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json");
        std::shared_ptr<Level> overworld = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/overworld.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveOverWorld.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveGame.json");
        std::shared_ptr<Level> town1 = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/town.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveOverWorld.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveGame.json");
        std::shared_ptr<Battle> battle = std::make_shared<Battle>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/battle.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveGame.json");
        std::shared_ptr<Scene> savescene = std::make_shared<SaveScene>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/save.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveGame.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveOverWorld.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json", true, overworld, battle);
        std::shared_ptr<Scene> gameOver = std::make_shared<GameOver>(app.GetScreenWidth(), app.GetScreenHeight());
        std::shared_ptr<Scene> loadscene = std::make_shared<SaveScene>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/areas/save.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveGame.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveOverWorld.json", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/savestate/saveBattle.json", false, overworld, battle);
        std::shared_ptr<Scene> level = overworld;
        std::shared_ptr<Scene> battleScene = battle;

        app.AddScene("title", title);
        app.AddScene("savescene", savescene);
        app.AddScene("loadscene", loadscene);
        app.AddScene("overworld", level);
        app.AddScene("town1", town1);
        app.AddScene("battle", battleScene);
        app.AddScene("gameOver", gameOver);
        app.SetScene("title");
        
        AssetManager::LoadShader("objectShader", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/basic.vert", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/basic.frag");
        AssetManager::LoadShader("textureShader", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/texture.vert", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/shaders/texture.frag");
        app.Run();
    }
    return 0;
}