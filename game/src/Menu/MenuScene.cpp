#include <Engine/Scene/Scene.h>

#include <Game/MenuScene.h>
#include <Game/MenuItem.h>
#include <Game/Obstacle.h>
#include <Game/Menu.h>
#include <Game/PlayerMenu.h>

MenuScene::MenuScene(float screenWidth, float screenHeight, std::string filepath, std::string battleFilePath, std::string root) : Scene(screenWidth, screenHeight), filepath(filepath), battleFilePath(battleFilePath), root(root)
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
    LoadMenuScene();
}

void MenuScene::LoadMenuScene()
{
    std::ifstream nextAreaFile(root + "/savestate/currentArea.json");
    if (!nextAreaFile.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json nextAreaJson;
    nextAreaFile >> nextAreaJson;
    nextScene = nextAreaJson["CurrentArea"];

    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j;
    file >> j;

    objectList.clear();
    objectMap.clear();

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

            if(texturePath.find("font") != std::string::npos)
            {
                texturePath = root + texturePath;
            }
            
        
            if(objs.key() == "menu"){
                if(name == "menu")
                {
                    menu = std::make_shared<PlayerMenu>(position, scale, color, "", name);
                    menu->SetCursorMinHeight(obst["cursorMinMaxHeight"][0]);
                    menu->SetCursorMaxHeight(obst["cursorMinMaxHeight"][1]);
                    go = menu;
                    AddObject(obst.value("name", "Unnamed"), go);
                    activeMenu = menu;
                }
                else if(name.find("menuItem") != std::string::npos && menu != nullptr)
                {
                    menu->AddMenuItem( obst.value("text", "Unnamed"), position, scale, color, texturePath, obst.value("text", "Unnamed"));
                    if(menu->GetCursorMaxHeight() < position.y)
                        menu->SetCursorMaxHeight(position.y);
                    if(menu->GetCursorMinHeight() > position.y)
                        menu->SetCursorMinHeight(position.y);
                }
                else if(name == "cursor" && menu != nullptr)
                {
                    menu->AddCursor(name, position, scale, color, texturePath);
                }
            }
            else if(objs.key() == "obstacles")
            {
                if(name == "background")
                {
                    //go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, texturePath, name, isStatic);
                    background = std::make_shared<Menu>(position, scale, color, "", name);
                    background->SetCursorMinHeight(200.0f);
                    background->SetCursorMaxHeight(600.0f);
                    background->SetActive(false);
                    go = background;
                    AddObject(obst.value("name", "Unnamed"), go);
                }
                else if(name.find("menuItem") != std::string::npos && background != nullptr)
                {
                    background->AddMenuItem( obst.value("text", "Unnamed"), position, scale, color, texturePath, obst.value("text", "Unnamed"));
                    if(background->GetCursorMaxHeight() < position.y)
                        background->SetCursorMaxHeight(position.y);
                    if(background->GetCursorMinHeight() > position.y)
                        background->SetCursorMinHeight(position.y);
                }
                else if(name == "cursor")
                {
                    background->AddCursor(name, position, scale, color, texturePath);
                }
            }
            else if(objs.key() == "playerMenu")
            {
                if(name.find("menuItem") != std::string::npos && menu != nullptr)
                {
                    menu->AddItemsMenuItem( obst.value("text", "Unnamed"), position, scale, color, texturePath, obst.value("text", "Unnamed"));
                    menu->AddItem(obst.value("text", "Unnamed"));
                }
            }
        }
    }
     
    camera.Create(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    
    leftScreenEdge = 0.0f;
    rightScreenEdge = screenWidth;
    bottomScreenEdge = 0.0f;
    topScreenEdge = screenHeight;

    menu->SetMenuItemsSize();
    menu->SetItemsMenuItemsSize();
    background->SetMenuItemsSize();
    PlayerInfo();
    SetPlayerMenu();
}

void MenuScene::OnEvent(const Input &input)
{
    if(input.IsKeyDown("Escape"))
    {
        EndScene(nextScene);
    }
    if(input.IsKeyDown("D"))
    {
        activeMenu = background;
        background->SetActive(true);
        menu->SetActive(false);
    }
    else if(input.IsKeyDown("A"))
    {
        activeMenu = menu;
        background->SetActive(false);
        menu->SetActive(true);
    }
    for(auto item : objectList)
    {
        item->OnEvent(input);
    }
}

void MenuScene::OnUpdate(const Input &input, PhysicsSystem &physics, float dt)
{
    if(menu->GetPlayerMove().find("UseItem") != std::string::npos)
    {
        UseItem();
    }
}

void MenuScene::PlayerInfo()
{
    std::ifstream battleSave(battleFilePath);
    if (!battleSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    battleSave >> saveData;
    glm::vec3 position;

    for(auto item : saveData.items())
    {
        if(item.key() == "Player")
        {
            playerInfoMap["HP"] = item.value()["hp"];
            playerInfoMap["Level"] = item.value()["level"];
            playerInfoMap["Strength"] = item.value()["strength"];
            playerInfoMap["XP"] = item.value()["xp"];
            for(auto conItem : item.value()["items"].items())
            {
                std::string conItemKey = conItem.key();
                items[conItem.key()] = item.value()["items"][conItem.key()];
                if(items[conItem.key()] < 1)
                {
                    menu->RemoveAllItems(conItem.key());
                }
            }
        }
    }
    battleSave.close();
}

void MenuScene::SetPlayerInfo()
{
    std::ofstream battleSave(battleFilePath);
    if (!battleSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    saveData["Player"] = nlohmann::json::object_t({
        {"hp", playerInfoMap["HP"]}, 
        {"level", playerInfoMap["Level"]},
        {"strength", playerInfoMap["Strength"]},
        {"xp", playerInfoMap["XP"]},
        {"items", items}
    });

    battleSave << saveData;
    battleSave.close();
}

void MenuScene::SetPlayerMenu()
{
    background->UpdateMenuItems(playerInfoMap);
}

void MenuScene::UseItem()
{
    std::string UseItemStr = "UseItem";
    std::string playerMove = menu->GetPlayerMove();
    std::string itemUse = playerMove.substr(UseItemStr.length(), playerMove.length() - UseItemStr.length());
    
    if(itemUse == "Potion")
    {
        playerInfoMap["HP"] += 5;
        items[itemUse]--;
        menu->SetPlayerMove("");
        menu->RemoveItem(itemUse);
        SetPlayerMenu();
        SetPlayerInfo();
    }
}