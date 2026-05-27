#include <Engine/Engine.h>
#include <Engine/Scene/Application.h>
#include <Game/Level.h>
#include <Game/TitleScene.h>
#include <Game/MenuScene.h>
#include <Game/SaveScene.h>
#include <Game/Battle.h>
#include <Game/GameOver.h>
#include <Game/Player.h>
#include <Game/Obstacle.h>
#include <iostream>

int main()
{
    
    Application app;
    const char* root = getenv("MYRPG_ROOT");

    
    if(!app.Create("Game", 1067, 800)){
        //1067 / 600 = 1.7783333
        std::cout << "Error Creating Engine" << std::endl;
    }
    else{
    
        auto quadMesh = std::make_shared<QuadMesh>();
        AssetManager::LoadMesh("quadMesh", quadMesh);
        auto batchMesh = std::make_shared<BatchMesh>();
        AssetManager::LoadMesh("batchMesh", batchMesh);
        
        std::shared_ptr<Scene> title = std::make_shared<TitleScene>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/title.json").c_str(), (std::string(root) + "/savestate/saveBattle.json").c_str(), std::string(root));
        std::shared_ptr<Level> overworld = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/overworld.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), (std::string(root) + "/savestate/saveBattle.json").c_str(), (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/currentArea.json").c_str(), std::string(root));
        std::shared_ptr<Level> town1 = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/town.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), std::string(root) + "/savestate/saveBattle.json", (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/currentArea.json").c_str(), std::string(root));
        std::shared_ptr<Level> cave1 = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/cave1.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), std::string(root) + "/savestate/saveBattle.json", (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/currentArea.json").c_str(), std::string(root));
        std::shared_ptr<Level> prancingpony = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/prancingpony.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), std::string(root) + "/savestate/saveBattle.json", (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/currentArea.json").c_str(), std::string(root));
        std::shared_ptr<Level> castle = std::make_shared<Level>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/castle.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), std::string(root) + "/savestate/saveBattle.json", (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/currentArea.json").c_str(), std::string(root));
        std::shared_ptr<Battle> battle = std::make_shared<Battle>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/battles/battle.json").c_str(), (std::string(root) + "/savestate/saveBattle.json").c_str(), (std::string(root) + "/savestate/saveGame.json").c_str(), std::string(root));
        std::shared_ptr<Scene> savescene = std::make_shared<SaveScene>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/save.json").c_str(), (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), (std::string(root) + "/savestate/saveBattle.json").c_str(), true, overworld, battle, std::string(root));
        std::shared_ptr<Scene> gameOver = std::make_shared<GameOver>(app.GetScreenWidth(), app.GetScreenHeight(), std::string(root));
        std::shared_ptr<Scene> loadscene = std::make_shared<SaveScene>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/save.json").c_str(), (std::string(root) + "/savestate/saveGame.json").c_str(), (std::string(root) + "/savestate/saveOverWorld.json").c_str(), (std::string(root) + "/savestate/saveBattle.json").c_str(), false, overworld, battle, std::string(root));
        std::shared_ptr<Scene> menu = std::make_shared<MenuScene>(app.GetScreenWidth(), app.GetScreenHeight(), (std::string(root) + "/areas/menu.json").c_str(), (std::string(root) + "/savestate/saveBattle.json").c_str(), std::string(root));
        std::shared_ptr<Scene> level = overworld;
        std::shared_ptr<Scene> battleScene = battle;

        app.AddScene("title", title);
        app.AddScene("savescene", savescene);
        app.AddScene("loadscene", loadscene);
        app.AddScene("menu", menu);
        app.AddScene("overworld", level);
        app.AddScene("town1", town1);
        app.AddScene("cave1", cave1);
        app.AddScene("prancingpony", prancingpony);
        app.AddScene("castle", castle);
        app.AddScene("battle", battleScene);
        app.AddScene("gameOver", gameOver);
        app.SetScene("title");
        
        AssetManager::LoadShader("objectShader", (std::string(root) + "/shaders/basic.vert").c_str(), (std::string(root) + "/shaders/basic.frag").c_str());
        AssetManager::LoadShader("textureShader", (std::string(root) + "/shaders/texture.vert").c_str(), (std::string(root) + "/shaders/texture.frag").c_str());
        app.Run();
    }
    return 0;
}