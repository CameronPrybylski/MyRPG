#include <Engine/Scene/Scene.h>

#include <Game/TitleScene.h>
#include <Game/TitleMenu.h>
#include <Game/Obstacle.h>


TitleScene::TitleScene(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath) : Scene(screenWidth, screenHeight), filepath(filepath)
{
    this->saveFilePath = saveFilePath;
    Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
    LoadTitle();
}

void TitleScene::LoadTitle()
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
                    menu = std::make_shared<TitleMenu>(position, scale, color, "", name);
                    menu->SetCursorMinHeight(obst["cursorMinMaxHeight"][0]);
                    menu->SetCursorMaxHeight(obst["cursorMinMaxHeight"][1]);
                    go = menu;
                    AddObject(obst.value("name", "Unnamed"), go);
                }
                else if(name.find("menuItem") != std::string::npos)
                {
                    if(position.y > cursorMax)
                    {
                        cursorMax = position.y;
                    }
                    if(position.y < cursorMin)
                    {
                        cursorMin = position.y;
                    }
                    menu->AddMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
                    menu->SetCursorMaxHeight(cursorMax);
                    menu->SetCursorMinHeight(cursorMin);
                }
                else if(name.find("loadMenuItem") != std::string::npos)
                {
                    if(position.y > cursorMax)
                    {
                        cursorMax = position.y;
                    }
                    if(position.y < cursorMin)
                    {
                        cursorMin = position.y;
                    }
                    menu->AddLoadMenuItem(name, position, scale, color, texturePath, obst.value("text", "Unnamed"));
                    menu->SetCursorMaxHeight(cursorMax);
                    menu->SetCursorMinHeight(cursorMin);
                }
                else if(name == "cursor" && menu != nullptr)
                {
                    menu->AddCursor(name, position, scale, color, texturePath);
                }
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
        //LoadPlayerInfo();
    }
}

void TitleScene::LoadPhysics(PhysicsSystem& physics)
{
    glm::vec3 gravity = {0.0f, 0.0f, 0.0f};
    physics.SetGravity(gravity);
    for(auto obj : objectList)
    {
        physics.RegisterBody(obj->transform, obj->rigidBody, obj->name);
    }
}

void TitleScene::OnEvent(const Input& input)
{
    for(auto& obj : objectList)
    {
        obj->OnEvent(input);
    }
}

void TitleScene::OnUpdate(const Input& input, PhysicsSystem& physics, float dt)
{
    //std::vector<CollisionEvent> collisions = physics.Update(dt);
    //OnCollision(collisions, dt);

    for(auto& obj : objectList)
    {
        obj->Update(input, dt);
    }

    if(menu->IsNewGame())
    {
        EndScene("overworld");
    }
    if(menu->IsLoadGame())
    {
        EndScene("loadscene");
    }

}

void TitleScene::OnCollision(std::vector<CollisionEvent> collisions, float dt)
{
}

void TitleScene::UpdateCamera()
{
    glm::vec3 playerPositionChange(0.0f);
    camera.OnUpdate(playerPositionChange);
}


