#include <Engine/Scene/Scene.h>

#include <Game/Battle.h>
#include <Game/BattleMenu.h>
#include <Game/PlayerInBattle.h>
#include <Game/Obstacle.h>
#include <Game/EnemyInBattle.h>

Battle::Battle(float screenWidth, float screenHeight, std::string filepath) : Scene(screenWidth, screenHeight), filepath(filepath)
{
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
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json j;
    file >> j;

    objectList.clear();
    objectMap.clear();
    player = nullptr;
    //std::cout << j["levelParams"]["completionDist"] << std::endl;
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
            
            if(objs.key() == "obstacles"){
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, "", name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
            else if(objs.key() == "player"){
                player = std::make_shared<PlayerInBattle>(position, scale, color, texturePath, name, isStatic);
                go = player;
                AddObject(obst.value("name", "Unnamed"), go);
                go = std::make_shared<Sword>(position, glm::vec3(180.0f, 180.0f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/textures/sword.png", "sword" ,false);
                player->AddItem("sword", go);
                AddObject("sword", go);
            }
            else if(objs.key() == "enemies"){
                std::shared_ptr<EnemyInBattle> enemy = std::make_shared<EnemyInBattle>(position, scale, color, texturePath, name);
                go = enemy;
                AddObject(obst.value("name", "Unnamed"), go);
                enemies[name] = enemy;
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
                else if(name == "cursor" && menu != nullptr)
                {
                    menu->AddCursor(name, position, scale, color, texturePath);
                }
            }
            /*
            else if(objs.key() == "aground"){
                glm::vec3 rotation = {obst["rotation"][0], obst["rotation"][1], obst["rotation"][2]};
                go = std::make_shared<Obstacle>(position, scale, rotation, velocity, color, texturePath, name, isStatic);
                AddObject(obst.value("name", "Unnamed"), go);
            }
            */
        }
    }
    camera.Create(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    
    leftScreenEdge = 0.0f;
    rightScreenEdge = screenWidth;
    bottomScreenEdge = 0.0f;
    topScreenEdge = screenHeight;

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
            playerMove = true;
        }
        if(!enemy.second->IsAlive())
        {
            deadEnemies.push_back(enemy.second->name);
        }
        enemy.second->SetMove(!playerMove);
    }

    if(deadEnemies.size() > 0)
        menu->SetDeadEnemies(deadEnemies);
    
    if(deadEnemies.size() == enemies.size())
    {
        EndScene("overworld");
    }
    
    //UpdateCamera();
}

void Battle::OnCollision(std::vector<CollisionEvent> collisions, float dt)
{

}

void Battle::HandlePlayerMove()
{
    player->SetMove(menu->GetPlayerMove());
    if(menu->GetPlayerMove().find("Attack") != std::string::npos)
    {
        enemies[menu->GetPlayerMove().substr(6)]->TakeDamage(player->GetAttackDamage());
    }
    playerMove = false;
}

void Battle::UpdateCamera()
{
    glm::vec3 playerPositionChange(0.0f);
    
    camera.OnUpdate(playerPositionChange);

}