#include <Game/Level.h>
#include <Game/Obstacle.h>
#include <Game/SaveSpot.h>

Level::Level(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveBattleFilePath, std::string saveGameFilePath) : Scene(screenWidth, screenHeight), filepath(filepath)
{
    this->saveFilePath = saveFilePath;
    this->saveBattleFilePath = saveBattleFilePath;
    this->saveGameFilePath = saveGameFilePath;
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
    completionDist = j["levelParams"]["completionDist"];
    nextLevel = j["levelParams"]["nextLevel"];
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
                player = std::make_shared<Player>(position, scale, color, texturePath, name, isStatic);
                go = player;
                AddObject(obst.value("name", "Unnamed"), go);
                go = std::make_shared<Sword>(position, glm::vec3(45.0f, 45.0f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/textures/sword.png", "sword" ,false);
                player->AddItem("sword", go);
                AddObject("sword", go);
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
                std::shared_ptr<SaveSpot> saveSpot = std::make_shared<SaveSpot>(position, scale, color, "", name);
                go = saveSpot;
                AddObject(obst.value("name", "Unnamed"), go);
                saveSpots[name] = saveSpot;
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

    gameOver = false;
    if(!initialStart)
    {
        LoadState();
    }

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
    for(auto& obj : objectMap)
    {
        obj.second->OnEvent(input);
    }
}

void Level::OnUpdate(const Input& input, PhysicsSystem &physics, float dt)
{
    //UpdatePhysics(physics, dt);
    std::vector<CollisionEvent> collisions = physics.Update(dt);
    OnCollision(collisions, dt);

    UpdateCamera();

    for(auto& obj : objectList)
    {
        obj->Update(input, dt);
    }

    if(player->alive == false)
    {
        gameOver = true;
    }
    if(player->inBattle)
    {
        SaveState();
        initialStart = false;
        EndScene("battle");
    }
    if(gameOver)
    {
        EndScene("gameOver");
    }
    if(player->transform.position.x + player->transform.scale.x >= completionDist){
        //EndScene(nextLevel);
    }
    if(saveSpots["saveSpot1"]->GetSaveGame())
    {
        SaveGame();
        saveSpots["saveSpot1"]->SetSaveGame(false);
    }
}

void Level::OnCollision(std::vector<CollisionEvent> collisions, float dt)
{
    for(auto& collision : collisions)
    {
        auto& obj1 = objectMap[collision.body1.id];
        auto& obj2 = objectMap[collision.body2.id];

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

void Level::SaveState()
{
    std::ofstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json saveData;
    nlohmann::json::array_t position = {player->transform.position.x, player->transform.position.y, player->transform.position.z};
    saveData["Player"] = nlohmann::json::object_t({{"position", position}, {"hp", player->hp}});
    saveData["Camera"] = nlohmann::json::object_t({{"leftScreenEdge", leftScreenEdge}, {"rightScreenEdge", rightScreenEdge}, {"topScreenEdge", topScreenEdge}, {"bottomScreenEdge", bottomScreenEdge}});
    saveData["Enemy"] = nlohmann::json::object_t({{"enemyFighting", player->enemyFighting}});
    levelSave << saveData;
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
    for(auto item : saveData.items())
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
        }
        else if(item.key() == "Enemy")
        {
            RemoveEnemy(item.value()["enemyFighting"]);
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

void Level::SaveGame()
{
    SaveState();
    std::ifstream battleSave(saveBattleFilePath);
    if (!battleSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    std::ifstream levelSave(saveFilePath);
    if (!levelSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }

    nlohmann::json loadBattleData;
    nlohmann::json saveData;
    battleSave >> loadBattleData;
    for(auto item : loadBattleData.items())
    {
        if(item.key() == "Player")
        {
           saveData["PlayerBattle"] = nlohmann::json::object_t({{"hp", item.value()["hp"]}});
        }
    }
    nlohmann::json loadLevelData;
    levelSave >> loadLevelData;

    glm::vec3 position;
    for(auto item : loadLevelData.items())
    {
        if(item.key() == "Camera")
        {
            saveData["Camera"] = nlohmann::json::object_t({
                {"leftScreenEdge", item.value()["leftScreenEdge"]},
                {"rightScreenEdge", item.value()["rightScreenEdge"]},
                {"topScreenEdge", item.value()["topScreenEdge"]},
                {"bottomScreenEdge", item.value()["bottomScreenEdge"]}
            });
        }
        else if(item.key() == "Player")
        {
            saveData["PlayerOverworld"] = nlohmann::json::object_t({
                {"position", item.value()["position"]},
                {"hp", player->hp}
            });
        }
        else if(item.key() == "Enemies")
        {
            //RemoveEnemy(item.value()["enemyFighting"]);
        }
    }

    std::ofstream gameSave(saveGameFilePath);
    if (!gameSave.is_open()) {
        throw std::runtime_error("Failed to open level file.");
    }
    
    gameSave << saveData;
    levelSave.close();
    battleSave.close();
    gameSave.close();
    
}