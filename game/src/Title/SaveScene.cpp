#include <Engine/Scene/Scene.h>

#include <Game/SaveScene.h>

SaveScene::SaveScene(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath) : Scene(screenWidth, screenHeight),
 saveFilePath(saveFilePath)
{

}

SaveScene::~SaveScene()
{
}

void SaveScene::Init()
{
}

void SaveScene::OnEvent(const Input &input)
{
}

void SaveScene::OnUpdate(const Input &input, PhysicsSystem &physics, float dt)
{
}
