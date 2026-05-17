#include <Engine/Scene/Scene.h>

#include <Game/Battle.h>
#include <Game/BattleMenu.h>
#include <Game/PlayerInBattle.h>
#include <Game/Obstacle.h>
#include <Game/EnemyInBattle.h>
#include <Game/Goblin.h>
#include <Game/Potion.h>
#include <Game/Ether.h>
#include <Game/Spell.h>
#include <Game/MenuItem.h>

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

    enemies.clear();

    std::ifstream nextAreaFile(root + "/savestate/currentArea.json");
    if (!nextAreaFile.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json nextAreaJson;
    nextAreaFile >> nextAreaJson;
    nextArea = nextAreaJson["CurrentArea"];

    nextAreaFile.close();

    std::ifstream nextAreaBackground(root + "/areas/" + nextArea + ".json");
    if (!nextAreaBackground.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json nextAreaBackgroundJson;
    nextAreaBackground >> nextAreaBackgroundJson;
    glm::vec4 backgroundColor;
    std::string backgroundTexturePath = "";
    objectList.clear();
    objectMap.clear();
    for(auto objs : nextAreaBackgroundJson["objects"]["aground"].items())
    {
        if(objs.value().contains("name") && objs.value().at("name") == "background")
        {
            backgroundColor = { objs.value().at("color")[0], 
                                objs.value().at("color")[1], 
                                objs.value().at("color")[2], 
                                objs.value().at("color")[3]
                            };
            backgroundTexturePath = objs.value().at("battleTexturePath");
        }
        if(objs.value().contains("name") && objs.value().at("name") == "top")
        {
            auto obst = objs.value();
            std::shared_ptr<GameObject> go;
            std::string name = obst.value("name", "Unnamed");
            glm::vec3 position = { obst["position"][0], obst["position"][1], obst["position"][2]};
            glm::vec3 scale = { obst["scale"][0], obst["scale"][1], obst["scale"][2]};
            glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
            glm::vec3 velocity = { obst["velocity"][0], obst["velocity"][1], obst["velocity"][2]};
            glm::vec4 color = { obst["color"][0], obst["color"][1], obst["color"][2], obst["color"][3]};
            bool isStatic = obst.value("isStatic", false);
            std::string texturePath = obst.value("texturePath", "Unnamed");
            if(texturePath != "")
            {
                texturePath = root + texturePath;
            }
            go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, texturePath, name, isStatic);
            AddObject(obst.value("name", "Unnamed"), go);
        }
    }
    nextAreaBackground.close();

    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j;
    file >> j;

    player = nullptr;
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
                    menu->GetMenuItem(name)->SetSelectable(obst.value("selectable", false));
                }
                else if(name.find("magicMenuItem") != std::string::npos)
                {
                    menu->AddMagicMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
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
        }
    }



    file.close();

    std::ifstream file2(newFilePath);
    if (!file2.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j2;
    file2 >> j2;

    for (const auto& objs : j2["objects"].items()) {
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
            
            if(objs.key() == "enemies"){
                int attackDamage = obst.value("attackDamage", 0);
                int xp = obst.value("xp", 0);
                std::shared_ptr<EnemyInBattle> enemy = CreateEnemy(position, scale, color, texturePath, name, attackDamage, xp);
                AddEnemy(enemy);
            }
            else if(objs.key() == "player"){
                player = std::make_shared<PlayerInBattle>(position, scale, color, texturePath, name, isStatic);
                go = player;
                AddObject(obst.value("name", "Unnamed"), go);
                for(const auto& spell : obst["spell"].items())
                {
                    player->AddSpell(spell.key(), std::make_shared<Spell>(spell.value()["Damage"], spell.value()["MPCost"]));
                }
                
                player->SetDeathTexture1Path(root + obst.value("deathTexture1Path", "Unamed"));
                player->SetDeathTexture2Path(root + obst.value("deathTexture2Path", "Unamed"));
            }
            else if(objs.key() == "menu"){
                if(name.find("attackMenuItem") != std::string::npos)
                {
                    menu->AddAttackMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
                }
            }
            else if(objs.key() == "aground")
            {
                if(backgroundTexturePath != "")
                {
                    texturePath = root + backgroundTexturePath;
                }
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, backgroundColor, texturePath, name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
        }
    }

    camera.Create(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    
    leftScreenEdge = 0.0f;
    rightScreenEdge = screenWidth;
    bottomScreenEdge = 0.0f;
    topScreenEdge = screenHeight;

    // Sort using a lambda that dereferences the shared pointers
    std::sort(objectList.begin(), objectList.end(), [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {
        return a->transform.position.z < b->transform.position.z; // Access members using the arrow operator
    });

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
    if(playerMove && player->GetHP() > 0 && !player->IsMove())
    {
        for(auto& obj : objectList)
        {
            obj->OnEvent(input);
        }
    }
}

void Battle::OnUpdate(const Input& input, PhysicsSystem& physics, float dt)
{
    if(player->GetHP() <= 0 && !player->DeathInProgress() && !player->DeathOver())
    {
        player->SetDeathInProgress(true);
    }
    if(player->DeathInProgress() && !player->DeathOver())
    {
        player->Update(input, dt);
        return;
    }
    if(player->IsMove())
    {
        player->Update(input, dt);
        if(player->IsAttack() && playerMove)
        {
            HandlePlayerMove();
            for(auto& obj : objectList)
            {
                obj->Update(input, dt);
            }
            if(!(menu->GetMagicMove().find("Magic") != std::string::npos && menu->GetPlayerMove().find("Attack") == std::string::npos))
            {
                menu->SetPlayerMove("");
                menu->SetMagicMove("");
            }
            player->SetMove(menu->GetPlayerMove());
            playerMove = false;
        }
        return;
    }

    std::vector<std::string> deadEnemies;
    if(player->GetHP() <= 0 && player->DeathOver())
    {
        initialStart = true;
        enemies.clear();
        deadEnemies.clear();
        player->SetDeathOver(false);
        player->SetDeathInProgress(false);
        Init();
        EndScene("gameOver");
    }

    std::vector<CollisionEvent> collisions = physics.Update(dt);
    OnCollision(collisions, dt);
    
    if(playerMove && menu->GetPlayerMove() != "" && !player->IsMove())
    {
        player->MakeMove(menu->GetPlayerMove());
        for(auto itr = enemies.begin(); itr != enemies.end();)
        {
            auto enemy = itr;
            enemy->second->SetMove(true);
            ++itr;
        }
        return;
    }

    for(auto& obj : objectList)
    {
        obj->Update(input, dt);
    }

    if(!(menu->GetPlayerMove().find("Magic") != std::string::npos && menu->GetPlayerMove().find("Attack") == std::string::npos))
    {
        menu->SetPlayerMove("");
    }
    player->SetMove(menu->GetPlayerMove());

   
    for(auto itr = enemies.begin(); itr != enemies.end();)
    {
        auto enemy = itr;
        if(enemy->second->GetMove())
        {
            HandleEnemyMove(enemy->second);
            playerMove = true;
        }
        if(!enemy->second->IsAlive())
        {
            deadEnemies.push_back(enemy->second->name);
            player->AddToXP(enemy->second->GetXP());
            player->CheckXP();
            //erase returns iterator to the next element in map
            itr = enemies.erase(itr);
        }
        else
        {
            enemy->second->SetMove(!playerMove);
            ++itr;
        }
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
        EndScene(nextArea);
    }
    
    menu->UpdatePlayerHP(player->GetHP());
    menu->UpdatePlayerMP(player->GetMP());
    
}

void Battle::OnCollision(std::vector<CollisionEvent> collisions, float dt)
{

}

void Battle::HandlePlayerMove()
{
    player->SetMove(menu->GetPlayerMove());
    if(menu->GetPlayerMove().find("Attack") != std::string::npos && menu->GetMagicMove().find("Magic") == std::string::npos)
    {
        if(enemies.find(menu->GetPlayerMove().substr(6)) != enemies.end())
        {
            enemies[menu->GetPlayerMove().substr(6)]->TakeDamage(player->GetAttackDamage());
        }
        menu->SetPlayerMove("");
    }
    else if(menu->GetMagicMove().find("Magic") != std::string::npos )
    {
        if(menu->GetPlayerMove().find("Attack") != std::string::npos)
        {
            size_t indexOfAttack = menu->GetPlayerMove().find("Attack");
            int indexOfEnemy = indexOfAttack + 6;
            if(enemies.find(menu->GetPlayerMove().substr(indexOfEnemy)) != enemies.end())
            {
                std::string magicType = menu->GetMagicMove().substr(5);
                int magicDamage = player->GetMagicDamage(magicType);
                if(magicDamage == 0)
                {
                    return;
                }
                else
                {
                    enemies[menu->GetPlayerMove().substr(indexOfEnemy)]->TakeDamage(magicDamage);
                }
            }
        }
        else if( menu->GetPlayerMove().find("Magic") != std::string::npos )
        {
            std::string magicType = menu->GetMagicMove().substr(5);
            player->UseMagic(magicType);
        }
        menu->SetPlayerMove("");
        menu->SetMagicMove("");
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
        menu->SetPlayerMove("");
    }
    playerMove = false;
}

void Battle::HandleEnemyMove(std::shared_ptr<EnemyInBattle> enemy)
{
    enemy->MakeMove();
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
        player->AddConsumableItem("Ether", std::make_shared<Ether>());
    }
}

void Battle::SavePlayerInfo()
{
    std::ofstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    
    saveData["Player"] = nlohmann::json::object_t({
        {"hp", player->GetHP()},
        {"mp", player->GetMP()},
        {"maxhp", player->GetMaxHP()},
        {"maxmp", player->GetMaxMP()},
        {"level", player->GetLevel()},
        {"strength", player->GetStrength()},
        {"xp", player->GetXP()},
        {"xpNeeded", player->GetXPNeeded()},
        {"items", nlohmann::json::object_t({{"Potion", player->ConsumableItemCount("Potion")}})},
        {"equippedWeapon", nlohmann::json::object_t({{player->GetEquippedWeapon()->GetName(), player->GetEquippedWeapon()->GetDamage()}})},
        {"weapons", nlohmann::json::object_t({{player->GetEquippedWeapon()->GetName(), player->GetEquippedWeapon()->GetDamage()}})},
        {"gil", player->GetGil()}
    });

    SavePlayerItems(saveData);
    
    levelSave << saveData;
    initialStart = false;
}

void Battle::SavePlayerItems(nlohmann::json& saveData)
{
    std::unordered_map<std::string, std::shared_ptr<Weapon>> weapons = player->GetWeapons();
    std::unordered_map<std::string, int> weaponDamages;

    for(auto weapon = weapons.begin(); weapon != weapons.end(); weapon++)
    {
        weaponDamages[weapon->second->GetName()] = weapon->second->GetDamage();
    }

    saveData["Player"]["weapons"] = weaponDamages;

    std::unordered_map<std::string, std::vector<std::shared_ptr<ConsumableItem>>> conItems = player->GetConsumableItems();
    std::unordered_map<std::string, int> conItemCount;

    for(auto item = conItems.begin(); item != conItems.end(); item++)
    {
        conItemCount[item->first] = item->second.size(); 
    }

    saveData["Player"]["items"] = conItemCount;
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
            player->SetMP(item.value()["mp"]);
            player->SetMaxHP(item.value()["maxhp"]);
            player->SetMaxMP(item.value()["maxmp"]);
            player->SetStrength(item.value()["strength"]);
            player->SetLevel(item.value()["level"]);
            player->SetXP(item.value()["xp"]);
            player->SetXPNeeded(item.value()["xpNeeded"]);
            player->SetGil(item.value()["gil"]);
            for(auto weapon : item.value()["equippedWeapon"].items())
            {
                std::string weaponKey = weapon.key();
                int weaponDamage = weapon.value();
                player->AddWeapon(weapon.key(), std::make_shared<Weapon>(weapon.value(), weapon.key()));
                player->ChangeWeapon(weaponKey);
            }
            for(auto weapon : item.value()["weapons"].items())
            {
                player->AddWeapon(weapon.key(), std::make_shared<Weapon>(weapon.value(), weapon.key()));
            }
            int itemCount = 0;
            int initialConsItemCnt = 0;
            std::string conItemKey = "";
            for(auto conItem : item.value()["items"].items())
            {
                conItemKey = conItem.key();
                itemCount = conItem.value();
                initialConsItemCnt = player->ConsumableItemCount(conItemKey);
                for(int i = 0; i < itemCount - initialConsItemCnt; i++)
                {
                    if(conItemKey == "Potion")
                    {
                        std::shared_ptr<ConsumableItem> conItem = std::make_shared<Potion>();
                        player->AddConsumableItem("Potion", conItem);
                    }
                    else if(conItemKey == "Ether")
                    {
                        std::shared_ptr<ConsumableItem> conItem = std::make_shared<Ether>();
                        player->AddConsumableItem("Ether", conItem);
                    }
                }
                menu->SetItemCount(conItemKey, itemCount);
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
            player->SetMP(item.value()["mp"]);
            player->SetMaxHP(item.value()["maxhp"]);
            player->SetMaxMP(item.value()["maxmp"]);
            player->SetStrength(item.value()["strength"]);
            player->SetLevel(item.value()["level"]);
            player->SetXP(item.value()["xp"]);
            player->SetXPNeeded(item.value()["xpNeeded"]);
            for(auto weapon : item.value()["equippedWeapon"].items())
            {
                std::string weaponKey = weapon.key();
                int weaponDamage = weapon.value();
                player->AddWeapon(weapon.key(), std::make_shared<Weapon>(weapon.value(), weapon.key()));
                player->ChangeWeapon(weaponKey);
            }
            for(auto weapon : item.value()["weapons"].items())
            {
                player->AddWeapon(weapon.key(), std::make_shared<Weapon>(weapon.value(), weapon.key()));
            }
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