#include <Engine/Engine.h>
#include <Engine/Scene/Application.h>
#include <Game/Level.h>
#include <Game/GameOver.h>
#include <Game/Player.h>
#include <Game/Obstacle.h>
#include <iostream>

int main()
{
    
    Application app;
    
    if(!app.Create("Game", 1067, 600)){
        //1067 / 600 = 1.7783333
        std::cout << "Error Creating Engine" << std::endl;
    }
    else{
    
        auto quadMesh = std::make_shared<QuadMesh>();
        AssetManager::LoadMesh("quadMesh", quadMesh);
        
        std::shared_ptr<Scene> level = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/levels/level1.json");
        //std::shared_ptr<Scene> level2 = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/levels/level2.json");
        std::shared_ptr<Scene> gameOver = std::make_shared<GameOver>(app.GetScreenWidth(), app.GetScreenHeight());

        app.AddScene("levelOne", level);
        //app.AddScene("levelTwo", level2);
        //app.AddScene("gameOver", gameOver);
        app.SetScene("levelOne");
        
        AssetManager::LoadShader("objectShader", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/shaders/basic.vert", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/shaders/basic.frag");
        AssetManager::LoadShader("textureShader", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/shaders/texture.vert", "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/shaders/texture.frag");
        app.Run();
    }
    return 0;
}