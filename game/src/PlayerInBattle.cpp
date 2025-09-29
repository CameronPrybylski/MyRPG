#include <Game/PlayerInBattle.h>

PlayerInBattle::PlayerInBattle(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic)
{
    mesh = AssetManager::GetMesh("quadMesh");
    shaderName = "textureShader";
    transform.position = position;
    transform.scale = scale;
    rigidBody.isStatic = isStatic;
    hp = 3;
    texture.Create("/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/textures/large_display_fighter.png");
    this->color = color;
    this->name = name;
}

PlayerInBattle::~PlayerInBattle()
{

}

void PlayerInBattle::OnEvent(const Input& input)
{
    
}

void PlayerInBattle::Update(const Input& input, float dt)
{
    PositionSword();
}

void PlayerInBattle::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if (collidedObj->name.find("enemy") != std::string::npos && !hit)
    {
        Hit(collisionNormal, dt);
        inBattle = true;
    }
}

void PlayerInBattle::Render(Renderer& renderer, const Camera& camera)
{
    renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    texture.Unbind();
}

void PlayerInBattle::Hit(glm::vec2 collisionNormal, float dt)
{
    //Collided from Left
    if(collisionNormal.x == 1 || collisionNormal.x == -1)
    {
        rigidBody.velocity.x *= -1;
    }
    else
    {
        rigidBody.velocity.y *= -1;
    }
    hit = true;
    timeSinceHit = 0.0f;
    hp--;
}

void PlayerInBattle::AddItem(std::string name, std::shared_ptr<GameObject> item)
{
    items[name] = item;
}

void PlayerInBattle::PositionSword()
{
    Transform swordTransform = items["sword"]->transform;
    swordTransform.position = transform.position;
    float swordScale = swordTransform.scale.x;
    if(usingSword)
    {
        if(positionFacing == "Down" || positionFacing == "Left")
        {
            swordScale *= -1;
        }
        if(positionFacing == "Right" || positionFacing == "Left")
        {
            swordTransform.position.x = transform.position.x + swordScale;
            if(positionFacing == "Right")
            {
                swordTransform.rotation.z = 180.0f;
            }
            else
            {
                swordTransform.rotation.z = 0.0f;
            }
        }
        else if(positionFacing == "Up" || positionFacing == "Down")
        {
            swordTransform.position.y = transform.position.y + swordScale;
            if(positionFacing == "Up")
            {
                swordTransform.rotation.z = 270.0f;
            }
            else
            {
                swordTransform.rotation.z = 90.0f;
            }
        }
    }
    items["sword"]->transform = swordTransform;
}
