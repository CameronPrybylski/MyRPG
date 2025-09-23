#include <Game/GameOver.h>
#include <Game/Obstacle.h>

GameOver::GameOver(float screenWidth, float screenHeight) : Scene(screenWidth, screenHeight)
{
    Init();
}

void GameOver::Init()
{
    std::shared_ptr<GameObject> obstacle = std::make_shared<Obstacle>(glm::vec3(screenWidth / 2, screenHeight / 2, 0.0f), 
    glm::vec3(250.0f, 50.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0, 1.0, 1.0, 1.0), "/Users/cameronprzybylski/Documents/C++/C++ Projects/MyPlatformer/textures/GameOver.png", "", false);

    camera.Create(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    objectList.clear();
    objectMap.clear();
    AddObject("obstacle_3", obstacle);
    leftScreenEdge = 0;
    rightScreenEdge = screenWidth;
    bottomScreenEdge = 0.0f;
    topScreenEdge = screenHeight;
}

void GameOver::OnEvent(const Input &input)
{
    if(input.IsKeyDown("space"))
    {
        EndScene("levelOne");
    }
}

void GameOver::OnUpdate(const Input &input, PhysicsSystem &physics, float dt)
{
    for(auto& obj : objectList)
    {
        obj->Update(dt);
    }
}

void GameOver::LoadPhysics(PhysicsSystem& physics)
{

}