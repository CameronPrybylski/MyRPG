#include <Game/Player.h>

Player::Player(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic)
{
    mesh = AssetManager::GetMesh("quadMesh");
    shaderName = "textureShader";
    transform.position = position;
    transform.scale = scale;
    rigidBody.isStatic = isStatic;
    hp = 3;
    texture.Create("/Users/cameronprzybylski/Documents/C++/C++ Projects/MyAdventureGame/textures/8bitLink.png");
    this->color = color;
    this->name = name;
}

Player::~Player()
{

}

void Player::OnEvent(const Input& input)
{
    if(input.IsKeyDown("D") && !hit)
    {
        rigidBody.velocity.x = 300.0f;
    }
    else if(input.IsKeyDown("A") && !hit)
    {
        rigidBody.velocity.x = -300.0f;
    }
    if(input.IsKeyDown("W") && !hit)
    {
        rigidBody.velocity.y = 300.0f;
    }
    else if(input.IsKeyDown("S") && !hit)
    {
        rigidBody.velocity.y = -300.0f;
    }
    if(input.IsKeyDown("L"))
    {
        usingSword = true;
        //items["sword"]->transform.position.x = transform.position.x + items["sword"]->transform.scale.x;
        //items["sword"]->transform.position.y = transform.position.y;
    }
    else
    {
        usingSword = false;
        //items["sword"]->transform.position.x = transform.position.x;
        //items["sword"]->transform.position.y = transform.position.y;
    }
    if(stop)
    {
        rigidBody.velocity.x = 0.0f;
        rigidBody.velocity.y = 0.0f;
    }
}

void Player::Update(const Input& input, float dt)
{
    if(hit)
    {
        if(timeSinceHit >= 0.5f){
            hit = false;
        }
        else{
            timeSinceHit += dt;
        }
    }
    else if(!(input.IsKeyDown("D") || input.IsKeyDown("A") || input.IsKeyDown("W") || input.IsKeyDown("S")))
    {
        rigidBody.velocity.x = 0;
        rigidBody.velocity.y = 0;
    }
    else
    {
        if(input.IsKeyDown("D") && !hit)
        {
            rigidBody.velocity.x = 300.0f;
            positionFacing = "Right";
        }
        else if(input.IsKeyDown("A") && !hit)
        {
            rigidBody.velocity.x = -300.0f;
            positionFacing = "Left";
        }
        else{
            rigidBody.velocity.x = 0.0f;
        }
        if(input.IsKeyDown("W") && !hit)
        {
            rigidBody.velocity.y = 300.0f;
            positionFacing = "Up";
        }
        else if(input.IsKeyDown("S") && !hit)
        {
            rigidBody.velocity.y = -300.0f;
            positionFacing = "Down";
        }
        else
        {
            rigidBody.velocity.y = 0.0f;
        }
    }
    if(stop)
    {
        rigidBody.velocity.x = 0.0f;
        rigidBody.velocity.y = 0.0f;
    }if(hp <= 0){
        alive = false;
    }

    PositionSword();
}

void Player::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if (collidedObj->name.find("enemy") != std::string::npos && !hit)
    {
        Hit(collisionNormal, dt);
    }
}

void Player::Render(Renderer& renderer, const Camera& camera)
{
    renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    texture.Unbind();
}

void Player::Hit(glm::vec2 collisionNormal, float dt)
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

void Player::AddItem(std::string name, std::shared_ptr<GameObject> item)
{
    items[name] = item;
}

void Player::PositionSword()
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
