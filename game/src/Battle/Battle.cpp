#include <Engine/Scene/Scene.h>

#include <Game/Battle.h>
#include <Game/BattleMenu.h>
#include <Game/PlayerInBattle.h>
#include <Game/Obstacle.h>
#include <Game/EnemyInBattle.h>
#include <Game/Goblin.h>
#include <Game/Potion.h>

#include <random>
#include <chrono>

Battle::Battle(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveGameFilePath, std::string root) : Scene(screenWidth, screenHeight), filepath(filepath), saveGameFilePath(saveGameFilePath), root(root)
{
    this->saveFilePath = saveFilePath;
    Init();
}

Battle::~Battle()
{

}

void Battle::Init()
{
    LoadBattle();
}

void Battle::LoadBattle()
{

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(1, 2);
    int random_num = distrib(gen);
    
    std::string newFilePath = filepath;
    int whereToInsert = 0;
    for(int i = 0; i < newFilePath.length(); i++)
    {
        if(newFilePath[i] == '.')
        {
            whereToInsert = i;
            break;
        }
    }

    newFilePath.insert(whereToInsert, std::to_string(random_num));

    if(random_num == 1)
    {
        playerMove = true;
    }
    else
    {
        playerMove = false;
    }
    
    std::ifstream file(newFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j;
    file >> j;

    objectList.clear();
    objectMap.clear();
    player = nullptr;
    nextArea = j["levelParams"]["nextLevel"];
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
            else if(texturePath.find("textures") != std::string::npos)
            {
                texturePath = root + texturePath;
            }
            
            if(objs.key() == "obstacles"){
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, "", name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
            else if(objs.key() == "player"){
                player = std::make_shared<PlayerInBattle>(position, scale, color, texturePath, name, isStatic);
                go = player;
                AddObject(obst.value("name", "Unnamed"), go);
                go = std::make_shared<Sword>(position, glm::vec3(180.0f, 180.0f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "", "sword" ,false);
                player->AddItem("sword", go);
                AddObject("sword", go);
            }
            else if(objs.key() == "enemies"){
                int attackDamage = obst.value("attackDamage", 0);
                int xp = obst.value("xp", 0);
                std::shared_ptr<EnemyInBattle> enemy = CreateEnemy(position, scale, color, texturePath, name, attackDamage, xp);
                AddEnemy(enemy);
            }
            else if(objs.key() == "menu"){
                if(name == "menu")
                {
                    menu = std::make_shared<BattleMenu>(position, scale, color, "", name);
                    menu->SetCursorMinHeight(obst["cursorMinMaxHeight"][0]);
                    menu->SetCursorMaxHeight(obst["cursorMinMaxHeight"][1]);
                    go = menu;
                    AddObject(obst.value("name", "Unnamed"), go);
                }
                else if(name.find("menuItem") != std::string::npos)
                {
                    menu->AddMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
                }
                else if(name.find("attackMenuItem") != std::string::npos)
                {
                    menu->AddAttackMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
                }
                else if(name.find("itemMenuItem") != std::string::npos)
                {
                    menu->AddItemMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
                }
                else if(name == "cursor" && menu != nullptr)
                {
                    menu->AddCursor(name, position, scale, color, texturePath);
                }
            }
            else if(objs.key() == "aground")
            {
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, texturePath, name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
        }
    }
    camera.Create(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    
    leftScreenEdge = 0.0f;
    rightScreenEdge = screenWidth;
    bottomScreenEdge = 0.0f;
    topScreenEdge = screenHeight;

    if(!initialStart)
    {
        LoadPlayerInfo();
    }
    else if(!loadBattle)
    {
        SavePlayerInfo();
    }
    if(loadBattle)
    {
        LoadGame();
    }

}

std::shared_ptr<EnemyInBattle> Battle::CreateEnemy(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, int attackDamage, int xp)
{
    if(name.find("Goblin") != std::string::npos)
    {
        return std::make_shared<Goblin>(position, scale, color, texturePath, name, attackDamage, xp);
    }
    else
    {
        return std::make_shared<EnemyInBattle>(position, scale, color, texturePath, name, attackDamage, xp);
    }
}

void Battle::AddEnemy(std::shared_ptr<EnemyInBattle> enemy)
{
    std::shared_ptr<GameObject> go;
    go = enemy;
    if(enemy->name.find("Goblin") != std::string::npos)
    {
        AddObject(enemy->name, go);
        enemies[enemy->name] = enemy;
    }
    else
    {
        AddObject(enemy->name, go);
        enemies[enemy->name] = enemy;
    }
}

void Battle::LoadPhysics(PhysicsSystem& physics)
{
    glm::vec3 gravity = {0.0f, 0.0f, 0.0f};
    physics.SetGravity(gravity);
    for(auto obj : objectList)
    {
        physics.RegisterBody(obj->transform, obj->rigidBody, obj->name);
    }
}

void Battle::OnEvent(const Input& input)
{
    if(playerMove)
    {
        for(auto& obj : objectList)
        {
            obj->OnEvent(input);
        }
    }
}

void Battle::OnUpdate(const Input& input, PhysicsSystem& physics, float dt)
{
    std::vector<CollisionEvent> collisions = physics.Update(dt);
    OnCollision(collisions, dt);
    if(playerMove && menu->GetPlayerMove() != "")
    {
        HandlePlayerMove();
    }

    for(auto& obj : objectList)
    {
        obj->Update(input, dt);
    }

    menu->SetPlayerMove("");
    player->SetMove(menu->GetPlayerMove());

    std::vector<std::string> deadEnemies;
    for(auto enemy : enemies)
    {
        if(enemy.second->GetMove())
        {
            HandleEnemyMove(enemy.second);
            playerMove = true;
        }
        if(!enemy.second->IsAlive())
        {
            deadEnemies.push_back(enemy.second->name);
            enemies.erase(enemy.first);
            player->AddToXP(enemy.second->GetXP());
            player->CheckXP();
        }
        enemy.second->SetMove(!playerMove);
    }

    if(deadEnemies.size() > 0)
    {
        menu->SetDeadEnemies(deadEnemies);
    }
    if(enemies.size() == 0)
    {
        LootBattle();
        SavePlayerInfo();
        initialStart = false;
        EndScene("overworld");
    }
    
    menu->UpdatePlayerHP(player->GetHP());
    if(player->GetHP() <= 0)
    {
        initialStart = true;
        enemies.clear();
        deadEnemies.clear();
        Init();
        EndScene("gameOver");
    }
}

void Battle::OnCollision(std::vector<CollisionEvent> collisions, float dt)
{

}

void Battle::HandlePlayerMove()
{
    player->SetMove(menu->GetPlayerMove());
    if(menu->GetPlayerMove().find("Attack") != std::string::npos)
    {
        if(enemies[menu->GetPlayerMove().substr(6)] != nullptr)
        {
            enemies[menu->GetPlayerMove().substr(6)]->TakeDamage(player->GetAttackDamage());
        }
        //enemies[menu->GetPlayerMove().substr(6)]->TakeDamage(player->GetAttackDamage());
    }
    else if(menu->GetPlayerMove().find("UseItem") != std::string::npos)
    {
        player->UseItem(menu->GetPlayerMove());
        std::string UseItemStr = "UseItem";
        std::string playerMove = player->GetMove();
        std::string itemUse = playerMove.substr(UseItemStr.length(), playerMove.length() - UseItemStr.length());
        
        if(player->ConsumableItemCount(itemUse) < 1)
        {
            menu->RemoveItemMenuItem(itemUse);
        }
    }
    playerMove = false;
}

void Battle::HandleEnemyMove(std::shared_ptr<EnemyInBattle> enemy)
{
    player->TakeDamage(enemy->GetAttackDamage());
}

void Battle::UpdateCamera()
{
    glm::vec3 playerPositionChange(0.0f);
    camera.OnUpdate(playerPositionChange);

}

void Battle::LootBattle()
{
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(1, 1);
    int random_num = distrib(gen);
    
    std::shared_ptr<ConsumableItem> conItem;
    std::string conItemStr = "";
    switch (random_num)
    {
        case 1:
            conItem = std::make_shared<Potion>();
            conItemStr = "Potion";
            break;
        
        default:
            break;
    }
    
    if(conItemStr != "")
    {
        player->AddConsumableItem(conItemStr, conItem);
    }
}

void Battle::SavePlayerInfo()
{
    std::ofstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    
    std::unordered_map<std::string, int> conItemMap;
    /*
    for(auto itr = player->GetConsumableItems().begin(); itr != player->GetConsumableItems().end(); itr++)
    {
        conItemMap[itr->first] = itr->second.size();
    }
    */
    saveData["Player"] = nlohmann::json::object_t({
        {"hp", player->GetHP()},
        {"maxhp", player->GetMaxHP()},
        {"level", player->GetLevel()},
        {"strength", player->GetStrength()},
        {"xp", player->GetXP()},
        {"xpNeeded", player->GetXPNeeded()},
        {"items", nlohmann::json::object_t({{"Potion", player->ConsumableItemCount("Potion")}})}
    });
    
    levelSave << saveData;
    initialStart = false;
}

void Battle::LoadPlayerInfo()
{
    std::ifstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    levelSave >> saveData;
    for(auto item : saveData.items())
    {
        if(item.key() == "Player")
        {
            player->SetHP(item.value()["hp"]);
            player->SetMaxHP(item.value()["maxhp"]);
            player->SetStrength(item.value()["strength"]);
            player->SetLevel(item.value()["level"]);
            player->SetXP(item.value()["xp"]);
            player->SetXPNeeded(item.value()["xpNeeded"]);
            int itemCount = 0;
            for(auto conItem : item.value()["items"].items())
            {
                std::string conItemKey = conItem.key();
                int itemCount = conItem.value();
                for(int i = 0; i < itemCount - player->ConsumableItemCount(conItemKey); i++)
                {
                    if(conItemKey == "Potion")
                    {
                        std::shared_ptr<ConsumableItem> conItem = std::make_shared<Potion>();
                        player->AddConsumableItem("Potion", conItem);
                    }
                }
            }
        }
    }
    if(player->ConsumableItemCount("Potion") < 1)
    {
        menu->RemoveItemMenuItem("Potion");
    }
}

void Battle::LoadGame()
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
        if(item.key() == "PlayerBattle")
        {
            player->SetHP(item.value()["hp"]);
            player->SetMaxHP(item.value()["maxhp"]);
            player->SetStrength(item.value()["strength"]);
            player->SetLevel(item.value()["level"]);
            player->SetXP(item.value()["xp"]);
            player->SetXPNeeded(item.value()["xpNeeded"]);
            for(auto conItem : item.value()["items"].items())
            {
                std::string conItemKey = conItem.key();
                int itemCount = item.value()["items"][conItem.key()];
                for(int i = 0; i < itemCount; i++)
                {
                    std::shared_ptr<ConsumableItem> conItem;
                    if(conItemKey == "Potion")
                    {
                        conItem = std::make_shared<Potion>();
                    }
                    player->AddConsumableItem(conItemKey, conItem);
                }
            }
        }
    }

    loadBattle = false;
    initialStart = false;
}