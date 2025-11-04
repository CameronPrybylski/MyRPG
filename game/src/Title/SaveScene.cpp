#include <Engine/Scene/Scene.h>

#include <Game/SaveScene.h>
#include <Game/Obstacle.h>
#include <Game/SaveMenu.h>
#include <Game/MenuItem.h>
#include <Game/Level.h>
#include <Game/Battle.h>

SaveScene::SaveScene(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveLevelFilePath, std::string saveBattleFilePath, bool save, std::shared_ptr<Level> loadGame, std::shared_ptr<Battle> loadBattle) : Scene(screenWidth, screenHeight),
 saveFilePath(saveFilePath), filepath(filepath), saveBattleFilePath(saveBattleFilePath), saveLevelFilePath(saveLevelFilePath), save(save), loadGame(loadGame), loadBattle(loadBattle)
{

}

SaveScene::~SaveScene()
{
}

void SaveScene::Init()
{
    LoadSaveScene();
}

void SaveScene::LoadSaveScene()
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j;
    file >> j;

    objectList.clear();
    objectMap.clear();
    //std::cout << j["levelParams"]["completionDist"] << std::endl;
    //nextArea = j["levelParams"]["nextLevel"];
    float cursorMin = screenHeight;
    float cursorMax = 0.0f;
    for (const auto& objs : j["objects"].items()) {
        for(const auto& obst : objs.value()){
            std::shared_ptr<GameObject> go;
            std::string name = obst.value("name", "Unnamed");
            glm::vec3 position = { obst["position"][0], obst["position"][1], obst["position"][2]};
            glm::vec3 scale = { obst["scale"][0], obst["scale"][1], obst["scale"][2]};
            glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
            glm::vec3 velocity = { obst["velocity"][0], obst["velocity"][1], obst["velocity"][2]};
            glm::vec4 color = { obst["color"][0], obst["color"][1], obst["color"][2], obst["color"][3]};
            bool isStatic = obst.value("isStatic", false);
            std::string texturePath = obst.value("texturePath", "Unnamed");
            
            if(objs.key() == "obstacles"){
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, "", name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
            else if(objs.key() == "menu"){
                if(name == "menu")
                {
                    menu = std::make_shared<SaveMenu>(position, scale, color, "", name);
                    menu->SetCursorMinHeight(obst["cursorMinMaxHeight"][0]);
                    menu->SetCursorMaxHeight(obst["cursorMinMaxHeight"][1]);
                    go = menu;
                    AddObject(obst.value("name", "Unnamed"), go);
                }
                else if(name.find("menuItem") != std::string::npos && menu != nullptr)
                {
                    menu->AddMenuItem( obst.value("text", "Unnamed"), position, scale, color, texturePath, obst.value("text", "Unnamed"));
                    menu->SetCursorMaxHeight(position.y);
                    menu->SetCursorMinHeight(position.y);
                }
                else if(name == "cursor" && menu != nullptr)
                {
                    menu->AddCursor(name, position, scale, color, texturePath);
                }
            }
        }
    }
     
    GetSavedGames();
    camera.Create(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    
    leftScreenEdge = 0.0f;
    rightScreenEdge = screenWidth;
    bottomScreenEdge = 0.0f;
    topScreenEdge = screenHeight;

}

void SaveScene::OnEvent(const Input &input)
{
    for(auto obj : objectList)
    {
        obj->OnEvent(input);
    }
}

void SaveScene::OnUpdate(const Input &input, PhysicsSystem &physics, float dt)
{
    for(auto& obj : objectList)
    {
        obj->Update(input, dt);
    }
    if(menu->SaveSlot() == "Back")
    {
        if(save)
        {
            EndScene("overworld");
        }
        else
        {
            EndScene("title");
        }
    }
    if(menu->SaveGame())
    {
        if(save)
        {
            SaveGame();
        }
        else
        {
            LoadGame();
        }
    }
}

void SaveScene::GetSavedGames()
{
    nlohmann::json saveData;
    std::ifstream gameSave(saveFilePath);
    if (!gameSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    gameSave >> saveData;
    int saveCount = 1;
    
    gameSave.close();
}

void SaveScene::SaveGame()
{
    std::ifstream battleSave(saveBattleFilePath);
    if (!battleSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    std::ifstream levelSave(saveLevelFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json loadBattleData;
    nlohmann::json saveData;
    battleSave >> loadBattleData;
    
    std::ifstream gameSave(saveFilePath);
    if (!gameSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    gameSave >> saveData;
    int saveCount = 0;
    for(auto item : saveData.items())
    {
        saveCount++;
    }
    gameSave.close();
    std::string saveNumber = menu->SaveSlot();
    saveData[saveNumber] = nlohmann::json::object_t();
    for(auto item : loadBattleData.items())
    {
        if(item.key() == "Player")
        {
           saveData[saveNumber]["PlayerBattle"] = nlohmann::json::object_t({{"hp", item.value()["hp"]}});
        }
    }
    nlohmann::json loadLevelData;
    levelSave >> loadLevelData;

    glm::vec3 position;
    for(auto item : loadLevelData.items())
    {
        if(item.key() == "Camera")
        {
            saveData[saveNumber]["Camera"] = nlohmann::json::object_t({
                {"leftScreenEdge", item.value()["leftScreenEdge"]},
                {"rightScreenEdge", item.value()["rightScreenEdge"]},
                {"topScreenEdge", item.value()["topScreenEdge"]},
                {"bottomScreenEdge", item.value()["bottomScreenEdge"]}
            });
        }
        else if(item.key() == "Player")
        {
            saveData[saveNumber]["PlayerOverworld"] = nlohmann::json::object_t({
                {"position", item.value()["position"]},
                {"hp", item.value()["hp"]}
            });
        }
    }

    std::ofstream gameSaveOut(saveFilePath);
    if (!gameSaveOut.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    
    gameSaveOut << saveData;
    levelSave.close();
    battleSave.close();
    gameSave.close();
    gameSaveOut.close();
    
}

void SaveScene::LoadGame()
{
    loadGame->SetSaveSlot(menu->SaveSlot());
    loadGame->SetLoadGame(true);
    loadBattle->SetSaveSlot(menu->SaveSlot());
    loadBattle->SetLoadGame(true);
    EndScene("overworld");
}
