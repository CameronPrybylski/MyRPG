#include <Game/Level.h>
#include <Game/Obstacle.h>
#include <Game/SaveSpot.h>
#include <Game/TreasureChest.h>
#include <Game/TownSpot.h>
#include <Game/NPC.h>
#include <Game/DialogueBox.h>
#include <Game/MenuItem.h>
#include <Game/MenuScene.h>
#include <Game/Menu.h>
#include <Game/Corridor.h>

bool Level::loadGame = false;
std::string Level::saveSlot = "";
bool Level::initialStart = true;

Level::Level(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveBattleFilePath, std::string saveGameFilePath, std::string currentAreaPath, std::string root) : Scene(screenWidth, screenHeight), filepath(filepath), root(root)
{
    this->saveFilePath = saveFilePath;
    this->saveBattleFilePath = saveBattleFilePath;
    this->saveGameFilePath = saveGameFilePath;
    this->currentAreaPath = currentAreaPath;
    initialStart = true;
    Init();
}

Level::~Level()
{

}

void Level::Init()
{
    LoadLevel(filepath);
}

void Level::LoadLevel(std::string filepath)
{
    std::ifstream areaFile(currentAreaPath);
    if (!areaFile.is_open()) {
        throw std::runtime_error("Failed to open area file.");
    }

    nlohmann::json areaJ;
    areaFile >> areaJ;

    std::string previousArea;
    if(areaJ.contains("PreviousArea"))
    {
        previousArea = areaJ["PreviousArea"];
    }
    else
    {
        previousArea = areaJ["CurrentArea"];
    }

    areaFile.close();

    std::string playerJsonPath = root + "/areas/global.json";
    LoadGlobal(playerJsonPath);

    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j;
    file >> j;
    file.close();

    completionDist = j["levelParams"]["completionDist"];
    nextLevel = j["levelParams"]["nextLevel"];
    savescene = j["levelParams"]["savescene"];
    areaName = j["levelParams"]["area"];
    combatArea = j["levelParams"]["combatArea"];
    moveCamera = j["levelParams"]["moveCamera"];
    glm::vec3 initialPosition = {0.0f, 0.0f, -1.0f};
    glm::vec3 playerInitialPosition = {0.0f, 0.0f, -1.0f};
    for (const auto& objs : j["objects"].items()) {
        for(const auto& obst : objs.value()){
            std::shared_ptr<GameObject> go;
            std::string name = obst.value("name", "Unnamed");
            glm::vec3 position = { obst["position"][0], obst["position"][1], obst["position"][2]};
            glm::vec3 scale, rotation, velocity;
            glm::vec4 color;
            bool isStatic;
            std::string texturePath;
            if(name != "player" && objs.key() != "treasureChests" && name != "dialogueBox" && objs.key() != "saveSpots")
            {
                position = { obst["position"][0], obst["position"][1], obst["position"][2]};
                scale = { obst["scale"][0], obst["scale"][1], obst["scale"][2]};
                rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                velocity = { obst["velocity"][0], obst["velocity"][1], obst["velocity"][2]};
                color = { obst["color"][0], obst["color"][1], obst["color"][2], obst["color"][3]};
                isStatic = obst.value("isStatic", false);
                texturePath = obst.value("texturePath", "Unnamed");
                if(texturePath.find("font") != std::string::npos)
                {
                    texturePath = root + texturePath;
                }
                else if(texturePath.find("textures") != std::string::npos)
                {
                    texturePath = root + texturePath;
                }
            }
            
            if(objs.key() == "obstacles"){
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, "", name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
            else if(objs.key() == "player"){
                if(initialPosition[2] == -1.0f)
                {
                    initialPosition = position;
                }
                playerInitialPosition = position;
                go = player;
                AddObject(obst.value("name", "Unnamed"), go);
            }
            else if(objs.key() == "enemies"){
                if(deadEnemies.count(name) == 0)
                {
                    std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(position, scale, velocity, color, "", name, isStatic);
                    go = enemy;
                    AddObject(obst.value("name", "Unnamed"), go);
                    enemies.push_back(enemy);
                }
            }
            else if(objs.key() == "saveSpots"){
                scale = { obst["scale"][0], obst["scale"][1], obst["scale"][2]};
                std::shared_ptr<SaveSpot> saveSpot = CreateSaveSpot( position, scale, name);
                go = saveSpot;
                AddObject(obst.value("name", "Unnamed"), go);
                saveSpots[name] = saveSpot;
            }
            else if(objs.key() == "treasureChests"){
                std::shared_ptr<TreasureChest> treasureChest = CreateTreasureChest(position, name);
                go = treasureChest;
                AddObject(obst.value("name", "Unnamed"), go);
                treasureChests[name] = treasureChest;
                std::unordered_map<std::string, int> contents = obst.at("contents");
                treasureChest->SetContents(contents);
            }
            else if(objs.key() == "towns"){
                std::shared_ptr<TownSpot> town = std::make_shared<TownSpot>(position, scale, color, texturePath, name);
                go = town;
                AddObject(obst.value("name", "Unnamed"), go);
                towns[name] = town;
                if(name == previousArea)
                {
                    initialPosition = position;
                }
            }
            else if(objs.key() == "npcs"){
                std::vector<std::string> dialogue = obst["dialogue"];
                float maxDist = obst["maxDist"];
                std::shared_ptr<NPC> npc = std::make_shared<NPC>(position, scale, velocity, color, texturePath, name, isStatic, dialogue, maxDist);
                std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree;
                if(obst.count("dialogueTree"))
                {
                    for(int i = 0; i < obst.at("dialogueTree").size(); i++)
                    {
                        dialogueTree.push_back(obst.at("dialogueTree")[i]);
                    }
                    npc->SetDialogueTree(dialogueTree);
                    if(dialogueBox != nullptr)
                    {
                        dialogueBox->SetDialogueTree(dialogueTree);
                    }
                }
                if(obst.contains("texturePath2") && texturePath != "")
                {
                    std::string texturePath2 = root + obst.value("texturePath2", "Unnamed");
                    npc->SetTexturePath2(texturePath2);
                }
                npcs[name] = npc;
                go = npc;
                AddObject(obst.value("name", "Unnamed"), go);
            }
            else if(objs.key() == "dialogue")
            {
                if(name == "selectMenu")
                {
                    dialogueBox->AddSelectMenu(position, scale, color, "", name);
                }
                else if(name.find("selectMenuItem") != std::string::npos && dialogueBox->GetSelectMenu() != nullptr)
                {
                    if(dialogueBox->GetSelectMenu()->GetNumberOfMenuItems() == 0)
                    {
                        dialogueBox->GetSelectMenu()->SetCursorMaxHeight(position.y);
                        dialogueBox->GetSelectMenu()->SetCursorMinHeight(position.y);
                    }
                    std::string text = obst.value("text", "Unnamed");
                    dialogueBox->AddSelectMenuItem(name, position, scale, color, texturePath, text);
                }
                else if(name == "cursor" && dialogueBox->GetSelectMenu() != nullptr)
                {
                    dialogueBox->GetSelectMenu()->AddCursor(name, position, scale, color, texturePath);
                }
            }
            else if(objs.key() == "aground"){
                if(name == "background")
                {
                    glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                    go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, texturePath, name, isStatic);
                    AddObject(obst.value("name", "Unnamed"), go);
                }
            }
            else if(objs.key() == "corridors")
            {
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                std::set<int> gaps;// = obst["gaps"];
                for(int i = 0; i < obst["gaps"].size(); i++)
                {
                    gaps.insert((int)obst["gaps"][i]);
                }
                std::shared_ptr<Corridor> corridor = std::make_shared<Corridor>(position, scale, rotation, velocity, color, "", name, isStatic, gaps);
                go = corridor;
                AddObject(obst.value("name", "Unnamed"), go);
                for(int i = 0; i < corridor->GetWalls().size(); i++)
                {
                    std::shared_ptr<GameObject> goi;
                    goi = corridor->GetWalls()[i];
                    AddObject(obst.value("name", "Unnamed") + std::to_string(i), goi);
                    corridor->GetWalls()[i]->name = obst.value("name", "Unnamed") + std::to_string(i);
                }
            }
        }
    }

    gameOver = false;
    if(!initialStart)
    {
        LoadState();
    }

    SaveArea(previousArea);

    // Sort using a lambda that dereferences the shared pointers
    std::sort(objectList.begin(), objectList.end(), [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {
        return a->transform.position.z < b->transform.position.z; // Access members using the arrow operator
    });

    player->transform.position.x = (leftScreenEdge + rightScreenEdge) / 2;
    player->transform.position.y = (bottomScreenEdge + topScreenEdge) / 2;

    if(enterArea && !initialStart)
    {
        player->transform.position = initialPosition;
    }
    else if(initialStart && !loadGame)
    {
        player->transform.position = playerInitialPosition;
    }
    else if(loadGame)
    {
        LoadGame();
        loadGame = false;
        initialStart = false;
    }

    float minX, maxX, minY, maxY;
    minX = player->transform.position.x - screenWidth / 2;
    maxX = player->transform.position.x + screenWidth / 2;
    minY = player->transform.position.y - screenHeight / 2;
    maxY = player->transform.position.y + screenHeight / 2;
    camera.Create(minX, maxX, minY, maxY, -1.0f, 1.0f);
    
    leftScreenEdge = minX;
    rightScreenEdge = maxX;
    bottomScreenEdge = minY;
    topScreenEdge = maxY;
}

void Level::LoadGlobal(std::string globalFilePath)
{
    objectList.clear();
    objectMap.clear();
    std::ifstream playerJsonFile(globalFilePath);
    if (!playerJsonFile.is_open()) {
        throw std::runtime_error("Failed to open player file.");
    }

    nlohmann::json playerJson;
    playerJsonFile >> playerJson;
    playerJsonFile.close();
    for(auto obj : playerJson["objects"].items())
    {
        nlohmann::json::object_t obst = obj.value();
        std::string name = obst["name"];
        glm::vec3 position = { obst["position"][0], obst["position"][1], obst["position"][2]};
        glm::vec3 scale = { obst["scale"][0], obst["scale"][1], obst["scale"][2]};
        glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
        glm::vec3 velocity = { obst["velocity"][0], obst["velocity"][1], obst["velocity"][2]};
        glm::vec4 color = { obst["color"][0], obst["color"][1], obst["color"][2], obst["color"][3]};
        bool isStatic = obst["isStatic"];
        std::string texturePath = obst["texturePath"];
        if(texturePath != "")
            texturePath = root + texturePath;

        if(name == "player")
        {
            LoadPlayer(position, scale, color, texturePath, name, isStatic);
            if(playerJson["objects"]["player"].contains("texturePath2"))
            {
                std::string texturePath2 = root + playerJson["objects"]["player"].value("texturePath2", "Unnamed");
                player->texturePath2 = texturePath2;
            }
            if(playerJson["objects"]["player"].contains("texturePathLeft"))
            {
                std::string texturePathLeft = root + playerJson["objects"]["player"].value("texturePathLeft", "Unnamed");
                player->texturePathLeft = texturePathLeft;
            }
            if(playerJson["objects"]["player"].contains("texturePathLeft2"))
            {
                std::string texturePathLeft2 = root + playerJson["objects"]["player"].value("texturePathLeft2", "Unnamed");
                player->texturePathLeft2 = texturePathLeft2;
            }
            if(playerJson["objects"]["player"].contains("texturePathRight"))
            {
                std::string texturePathRight = root + playerJson["objects"]["player"].value("texturePathRight", "Unnamed");
                player->texturePathRight = texturePathRight;
            }
            if(playerJson["objects"]["player"].contains("texturePathRight2"))
            {
                std::string texturePathRight2 = root + playerJson["objects"]["player"].value("texturePathRight2", "Unnamed");
                player->texturePathRight2 = texturePathRight2;
            }
            if(playerJson["objects"]["player"].contains("texturePathUp"))
            {
                std::string texturePathUp = root + playerJson["objects"]["player"].value("texturePathUp", "Unnamed");
                player->texturePathUp = texturePathUp;
            }
            if(playerJson["objects"]["player"].contains("texturePathUp2"))
            {
                std::string texturePathUp2 = root + playerJson["objects"]["player"].value("texturePathUp2", "Unnamed");
                player->texturePathUp2 = texturePathUp2;
            }
        }
        else if(name == "treasureChest")
        {
            LoadTreasureChest(position, scale, color, texturePath, name);
        }
        else if(name == "dialogueBox")
        {
            LoadDialogueBox(position, scale, color, texturePath, name);
        }
        else if(name == "textPos")
        {
            std::string text = obst["text"];
            std::shared_ptr<MenuItem> textPos = std::make_shared<MenuItem>(name, position, scale, color, texturePath, text);
            dialogueBox->SetCurrentText(textPos);
        }
        else if(name == "saveSpot")
        {
            saveSpotTemplate = std::make_shared<SaveSpot>(position, scale, color, texturePath, name);
        }
    }
}

void Level::LoadPlayer(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic)
{
    player = std::make_shared<Player>(position, scale, color, texturePath, name, isStatic);
}

void Level::LoadTreasureChest(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    treasureChestTemplate = std::make_shared<TreasureChest>(position, scale, color, texturePath, name);
}

void Level::LoadDialogueBox(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    dialogueBox = std::make_shared<DialogueBox>(position, scale, color, texturePath, name);
    std::shared_ptr<GameObject> go;
    go = dialogueBox;
    AddObject(name, go);
}

void Level::LoadPhysics(PhysicsSystem& physics)
{
    glm::vec3 gravity;
    gravity.x = 0.0f;
    gravity.z = 0.0f;
    gravity.y = 0.0f;
    physics.SetGravity(gravity);
    for(auto& obj : objectMap)
    {
        physics.RegisterBody(obj.second->transform, obj.second->rigidBody, obj.second->name);
    }
}

void Level::OnEvent(const Input &input)
{
    if(input.IsKeyDown("Escape"))
    {
        SaveState();
        EndScene("menu");
    }
    for(auto& obj : objectMap)
    {
        obj.second->OnEvent(input);
    }
    if(player->talkingToNPC && dialogueBox->InUse() && dialogueBox->HasSelectMenu())
    {
        std::unordered_map<std::string, std::shared_ptr<MenuItem>> menuItems = dialogueBox->GetSelectMenu()->GetMenuItems();
        for(auto menuItem : menuItems)
        {
            std::string menuItemText = menuItem.second->GetText();
            if(menuItemText.find("Pay") != std::string::npos)
            {
                int gil = player->GetGil();
                std::string strGil;
                size_t space = menuItemText.find(" ");
                strGil = menuItemText.substr(space+1, menuItemText.length());
                int iCost =  std::stoi(strGil);
                if(gil < iCost)
                {
                    menuItem.second->SetSelectable(false);
                }
                else
                {
                    menuItem.second->SetSelectable(true);
                }
            }
            else
            {
                menuItem.second->SetSelectable(true);
            }
        }
        if(dialogueBox->DialogueResponse("Pay") != "")
        {
            int gil = player->GetGil();
            int iCost =  std::stoi(dialogueBox->DialogueResponse("Pay"));
            if(gil >= iCost)
            {
                int newGil = gil - iCost;
                player->SetGil(newGil);
                dialogueBox->ClearDialogueResponses();
            }
        }
    }
}

void Level::OnUpdate(const Input& input, PhysicsSystem &physics, float dt)
{
    //UpdatePhysics(physics, dt);
    std::vector<CollisionEvent> collisions = physics.Update(dt);
    OnCollision(collisions, dt);
    initialStart = false;
    enterArea = false;

    if(moveCamera)
    {
        UpdateCamera();
    }

    for(auto& obj : objectList)
    {
        obj->Update(input, dt);
    }

    if(player->alive == false)
    {
        gameOver = true;
    }
    if(player->GetDistance() >= 1000.0f)
    {
        player->SetDistance(0.0f);
        if(combatArea)
        {
            SaveState();
            initialStart = false;
            EndScene("battle");
        }
    }
    if(gameOver)
    {
        EndScene("gameOver");
    }
    if(player->transform.position.x + player->transform.scale.x >= completionDist){
        //EndScene(nextLevel);
    }
    for(auto saveSpot : saveSpots)
    {
        if(saveSpot.second->GetSaveGame())
        {
            saveSpot.second->SetSaveGame(false);
            SaveGame(saveSpot.first);
        }
    }
    for(auto treasureChest = treasureChests.begin(); treasureChest != treasureChests.end(); treasureChest++)
    {
        if(treasureChest->second->GetOpenChest() && !treasureChest->second->Empty())
        {
            treasureChest->second->SetOpenChest(false);
            treasureChest->second->SetEmpty(true);
            OpenChest(treasureChest->first);
        }
    }
    for(auto town : towns)
    {
        if(town.second->EnterTown())
        {
            town.second->SetEnterTown(false);
            SaveState();
            PreviousArea();
            EndScene(town.first);
        }
    }
    if(player->talkingToNPC && !dialogueBox->InUse())
    {
        npcs[player->npcTalkingTo]->SetTalking(true);
        dialogueBox->SetDialogueTree(npcs[player->npcTalkingTo]->GetDialogueTree());
        dialogueBox->SetInUse(true);
        glm::vec3 newPosition = npcs[player->npcTalkingTo]->transform.position + glm::vec3(0.0f, 150.0f, 0.5f);
        dialogueBox->transform.position = newPosition;
        if(dialogueBox->HasSelectMenu())
            dialogueBox->GetSelectMenu()->ChangePosition( newPosition + glm::vec3(-200.0f, -100.0f, 0.0f), -25.0f, 50.0f);
        dialogueBox->SetTextPosition(newPosition + glm::vec3(10.0f, 0.0f, 0.0f));
        dialogueBox->SetBordersPosition(newPosition);
        dialogueBox->SetDialogue(npcs[player->npcTalkingTo]->GetDialogue());
    }
    if(dialogueBox != nullptr && dialogueBox->GetIndex() >= dialogueBox->GetDialogue().size() && dialogueBox->InUse())
    {
        npcs[player->npcTalkingTo]->SetTalking(false);
        dialogueBox->SetInUse(false);
        player->npcTalkingTo = "";
        player->talkingToNPC = false;
        dialogueBox->SetIndex(0);
        dialogueBox->SetIndexOfDTree(0);
        dialogueBox->ClearDialogueResponses();
    }
    
}

void Level::OnCollision(std::vector<CollisionEvent> collisions, float dt)
{
    bool playerContactWithNPC = false;
    for(auto& collision : collisions)
    {
        auto& obj1 = objectMap[collision.body1.id];
        auto& obj2 = objectMap[collision.body2.id];
        if(collision.body1.id == "player" || collision.body2.id == "player")
        {
            if(collision.body1.id.find("npc") != std::string::npos || collision.body2.id.find("npc") != std::string::npos)
            {
                playerContactWithNPC = true;
            }
        }
        if(!collision.body1.rigidBody->isStatic && collision.body2.rigidBody->isStatic)
        {
            obj2->OnCollision(obj1, collision.collisionNormalBody2, dt);
            obj1->OnCollision(obj2, collision.collisionNormalBody1, dt);
        }
        else
        {
            obj1->OnCollision(obj2, collision.collisionNormalBody1, dt);
            obj2->OnCollision(obj1, collision.collisionNormalBody2, dt);
        }
    }
    if(playerContactWithNPC)
    {
        player->contactWithNPC = true;
    }
    else{
        player->contactWithNPC = false;
    }
}

void Level::UpdateCamera()
{
    if(!initialStart)
    {
        //std::cout << "Hello" << std::endl;
    }
    glm::vec3 playerPositionChange(0.0f);
    float changeX = player->transform.position.x - player->rigidBody.previousPosition.x;
    float changeY = player->transform.position.y - player->rigidBody.previousPosition.y;
    if(changeX != 0.0f)
    {
        playerPositionChange.x = changeX;
    }
    if(changeY != 0)
    {
        playerPositionChange.y = changeY;
    }
    camera.OnUpdate(playerPositionChange);
    topScreenEdge += changeY;
    bottomScreenEdge += changeY;
    leftScreenEdge += changeX;
    rightScreenEdge += changeX;
    
}

std::shared_ptr<TreasureChest> Level::CreateTreasureChest(glm::vec3 position, std::string name)
{
    return std::make_shared<TreasureChest>(position, treasureChestTemplate->transform.scale, treasureChestTemplate->color, treasureChestTemplate->GetTexturePath(), name);
}

std::shared_ptr<SaveSpot> Level::CreateSaveSpot(glm::vec3 position, glm::vec3 scale, std::string name)
{
    return std::make_shared<SaveSpot>(position, scale, saveSpotTemplate->color, saveSpotTemplate->GetTexturePath(), name);
}

void Level::OpenChest(std::string treasureChestName)
{
    if(treasureChests.count(treasureChestName))
    {
        std::unordered_map<std::string, int> contents = treasureChests[treasureChestName]->GetContents();
        for(auto content : contents)
        {
            if(content.first == "Gil")
            {
                player->AddGil(content.second);
            }
            else if(content.first.find("Weapon") != std::string::npos)
            {
                std::string strWeapon = "Weapon";
                std::string weaponName = content.first.substr(strWeapon.length());
                AddWeapon(weaponName, content.second);
            }
        }
    }
}

void Level::AddWeapon(std::string weaponName, int weaponDamage)
{
    std::ifstream battleSaveIn(saveBattleFilePath);
    if (!battleSaveIn.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    battleSaveIn >> saveData;

    battleSaveIn.close();

    if(saveData["Player"]["weapons"][weaponName].size() == 0)
        saveData["Player"]["weapons"][weaponName] = weaponDamage;

    std::ofstream battleSaveOut(saveBattleFilePath);
    if (!battleSaveOut.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    battleSaveOut << saveData;

    battleSaveOut.close();
    std::string description = "Damage of " + std::to_string(weaponDamage);
    MenuScene::AddEquipmentItem(weaponName, description);

}

void Level::SaveState()
{
    std::ifstream loadState(saveFilePath);
    if (!loadState.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    
    nlohmann::json saveData;
    loadState >> saveData;
    player->transform.position = player->rigidBody.previousPosition;
    nlohmann::json::array_t position = {player->transform.position.x, player->transform.position.y, player->transform.position.z};
    saveData[areaName]["Player"] = nlohmann::json::object_t({{"position", position}, {"hp", player->hp}, {"gil", player->GetGil()}});
    saveData[areaName]["Camera"] = nlohmann::json::object_t({{"leftScreenEdge", leftScreenEdge}, {"rightScreenEdge", rightScreenEdge}, {"topScreenEdge", topScreenEdge}, {"bottomScreenEdge", bottomScreenEdge}});
    saveData[areaName]["Enemy"] = nlohmann::json::object_t({{"enemyFighting", player->enemyFighting}});

    for(auto item : saveData.items())
    {
        if(item.value().contains("Player") && item.value().at("Player").contains("gil"))
        {
            saveData[item.key()]["Player"]["gil"] = player->GetGil();
        }
    }

    std::unordered_map<std::string, bool> emptyChests;

    for(auto treasureChest = treasureChests.begin(); treasureChest != treasureChests.end(); treasureChest++)
    {
        emptyChests[treasureChest->first] = treasureChest->second->Empty();
    }

    saveData[areaName]["TreasureChests"] = emptyChests;

    std::ofstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    levelSave << saveData;
    

    std::ofstream currentArea(root + "/savestate/currentArea.json");
    if (!currentArea.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json currentAreaData;
    currentAreaData["CurrentArea"] = areaName;
    
    currentArea << currentAreaData;

    SaveBattleState();

}

void Level::SaveBattleState()
{
    std::ifstream battleSaveIn(saveBattleFilePath);
    if (!battleSaveIn.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    battleSaveIn >> saveData;
    battleSaveIn.close();

    std::ofstream battleSaveOut(saveBattleFilePath);
    if (!battleSaveOut.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    
    saveData["Player"]["gil"] = player->GetGil();

    battleSaveOut << saveData;
}

void Level::LoadState()
{
    std::ifstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    levelSave >> saveData;
    glm::vec3 position;

    for(auto item : saveData[areaName].items())
    {
        if(item.key() == "Camera")
        {
            leftScreenEdge = item.value()["leftScreenEdge"];
            rightScreenEdge = item.value()["rightScreenEdge"];
            topScreenEdge = item.value()["topScreenEdge"];
            bottomScreenEdge = item.value()["bottomScreenEdge"];
        }
        else if(item.key() == "Player")
        {
            position = {item.value()["position"][0], item.value()["position"][1], item.value()["position"][2]};
            player->SetHP(item.value()["hp"]);
            if(item.value().contains("gil"))
            {
                player->SetGil(item.value()["gil"]);
            }
        }
        else if(item.key() == "Enemy")
        {
            RemoveEnemy(item.value()["enemyFighting"]);
        }
        else if(item.key() == "TreasureChests")
        {
            for(auto treasureChest = treasureChests.begin(); treasureChest != treasureChests.end(); treasureChest++)
            {
                treasureChest->second->SetEmpty(item.value()[treasureChest->first]);
            }
        }
    }
    player->rigidBody.previousPosition = player->transform.position;
    player->transform.position = position;
    camera.Create(leftScreenEdge, rightScreenEdge, bottomScreenEdge, topScreenEdge, -1.0f, 1.0f);
}

void Level::RemoveEnemy(std::string enemyName)
{
    for(int i = 0; i < objectList.size(); i++)
    {
        if(objectList[i]->name == enemyName)
        {
            objectList.erase(objectList.begin() + i);
        }
    }
    objectMap.erase(enemyName);
    deadEnemies.insert(enemyName);
}

void Level::SaveGame(std::string saveSpot)
{
    SaveState();

    EndScene(savescene);
}

void Level::LoadGame()
{
    std::ifstream levelSave(saveGameFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    levelSave >> saveData;
    glm::vec3 position;
    nlohmann::json saveGame = saveData[saveSlot];
    for(auto item : saveGame.items())
    {
        if(item.key() == "Camera")
        {
            leftScreenEdge = item.value()["leftScreenEdge"];
            rightScreenEdge = item.value()["rightScreenEdge"];
            topScreenEdge = item.value()["topScreenEdge"];
            bottomScreenEdge = item.value()["bottomScreenEdge"];
        }
        else if(item.key() == "PlayerOverworld")
        {
            position = {item.value()["position"][0], item.value()["position"][1], item.value()["position"][2]};
            player->SetHP(item.value()["hp"]);
            if(item.value().contains("gil"))
            {
                player->SetGil(item.value()["gil"]);
            }
        }
        else if(item.key() == "Enemy")
        {
            RemoveEnemy(item.value()["enemyFighting"]);
        }
        else if(item.key() == "TreasureChests")
        {
            for(auto treasureChest = treasureChests.begin(); treasureChest != treasureChests.end(); treasureChest++)
            {
                treasureChest->second->SetEmpty(item.value()[treasureChest->first]);
            }
        }
    }
    player->rigidBody.previousPosition = player->transform.position;
    player->transform.position = position;
    camera.Create(leftScreenEdge, rightScreenEdge, bottomScreenEdge, topScreenEdge, -1.0f, 1.0f);
}

void Level::Reset()
{
    initialStart = true;
}

void Level::PreviousArea()
{
    std::ifstream currentAreaIn(currentAreaPath);
    if (!currentAreaIn.is_open()) {
        throw std::runtime_error("Failed to open area file.");
    }

    nlohmann::json currentAreaData;
    currentAreaIn >> currentAreaData;
    currentAreaData["PreviousArea"] = areaName;
    
    currentAreaIn.close();

    std::ofstream currentAreaOut(currentAreaPath);
    if (!currentAreaOut.is_open()) {
        throw std::runtime_error("Failed to open area file.");
    }

    currentAreaOut << currentAreaData;
    
    currentAreaOut.close();
}

void Level::SaveArea(std::string previousArea)
{
    std::ifstream currentAreaIn(currentAreaPath);
    if (!currentAreaIn.is_open()) {
        throw std::runtime_error("Failed to open area file.");
    }

    nlohmann::json currentAreaData;
    currentAreaIn >> currentAreaData;
    currentAreaData["CurrentArea"] = areaName;
    currentAreaData["PreviousArea"] = previousArea;
    
    currentAreaIn.close();

    std::ofstream currentAreaOut(currentAreaPath);
    if (!currentAreaOut.is_open()) {
        throw std::runtime_error("Failed to open area file.");
    }

    currentAreaOut << currentAreaData;
    
    currentAreaOut.close();
}