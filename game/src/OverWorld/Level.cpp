#include <Game/Level.h>
#include <Game/Obstacle.h>

Level::Level(float screenWidth, float screenHeight, std::string filepath) : Scene(screenWidth, screenHeight), filepath(filepath)
{
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
                std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(position, scale, velocity, color, "", name, isStatic);
                go = enemy;
                AddObject(obst.value("name", "Unnamed"), go);
                enemies.push_back(enemy);
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

}

void Level::LoadPhysics(PhysicsSystem& physics)
{
    glm::vec3 gravity;
    gravity.x = 0.0f;
    gravity.z = 0.0f;
    gravity.y = 0.0f;
    physics.SetGravity(gravity);
    for(auto& obj : objectList)
    {
        physics.RegisterBody(obj->transform, obj->rigidBody, obj->name);
    }
}

void Level::OnEvent(const Input &input)
{
    for(auto& obj : objectList)
    {
        obj->OnEvent(input);
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
        EndScene("battle");
    }
    if(gameOver)
    {
        EndScene("gameOver");
    }
    if(player->transform.position.x + player->transform.scale.x >= completionDist){
        //EndScene(nextLevel);
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