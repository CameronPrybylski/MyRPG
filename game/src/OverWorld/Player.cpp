#include <Game/Player.h>

Player::Player(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic)
{
    mesh = AssetManager::GetMesh("quadMesh");
    shaderName = "textureShader";
    transform.position = position;
    transform.scale = scale;
    rigidBody.isStatic = isStatic;
    rigidBody.mass = 1.0f;
    hp = 10;
    gil = 0;
    this->texturePath = texturePath;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
    this->color = color;
    this->name = name;
}

Player::~Player()
{

}

void Player::OnEvent(const Input& input)
{
    if(input.IsKeyDown("D") && !hit && !talkingToNPC)
    {
        rigidBody.velocity.x = 300.0f;
        rigidBody.velocity.y = 0.0f;
    }
    else if(input.IsKeyDown("A") && !hit && !talkingToNPC)
    {
        rigidBody.velocity.x = -300.0f;
        rigidBody.velocity.y = 0.0f;
    }
    else if(input.IsKeyDown("W") && !hit && !talkingToNPC)
    {
        rigidBody.velocity.y = 300.0f;
        rigidBody.velocity.x = 0.0f;
        leftDist = 0.0f;
        rightDist = 0.0f;
    }
    else if(input.IsKeyDown("S") && !hit && !talkingToNPC)
    {
        rigidBody.velocity.y = -300.0f;
        rigidBody.velocity.x = 0.0f;
        leftDist = 0.0f;
        rightDist = 0.0f;
    }
    else if(!(input.IsKeyDown("D") || input.IsKeyDown("A") || input.IsKeyDown("W") || input.IsKeyDown("S")))
    {
        rigidBody.velocity.x = 0;
        rigidBody.velocity.y = 0;
    }
    if(stop)
    {
        rigidBody.velocity.x = 0.0f;
        rigidBody.velocity.y = 0.0f;
    }
    if(input.IsKeyDown("SPACE") && contactWithNPC)
    {
        talkingToNPC = true;
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
    if(stop)
    {
        rigidBody.velocity.x = 0.0f;
        rigidBody.velocity.y = 0.0f;
    }
    if(hp <= 0)
    {
        alive = false;
    }

    SetDirection();
}

void Player::SetDirection()
{
    distance += std::abs(transform.position.x - rigidBody.previousPosition.x) + std::abs(transform.position.y - rigidBody.previousPosition.y);
    if(transform.position.x < rigidBody.previousPosition.x)
    {
        leftDist += std::abs(transform.position.x - rigidBody.previousPosition.x);
        rightDist = 0.0f;
        positionFacing = "LEFT";
        if(texturePathLeft != "")
        {
            texture.Delete();
            texture.Create(texturePathLeft);
            if(texturePathLeft2 != "" && leftDist > 60.0f)
            {
                std::string tempTextPath = texturePathLeft;
                texturePathLeft = texturePathLeft2;
                texturePathLeft2 = tempTextPath;
                leftDist = 0.0f;
            }
        }
    }
    else if(transform.position.x > rigidBody.previousPosition.x)
    {
        rightDist += std::abs(transform.position.x - rigidBody.previousPosition.x);
        leftDist = 0.0f;
        positionFacing = "RIGHT";
        if(texturePathRight != "")
        {
            texture.Delete();
            texture.Create(texturePathRight);
            if(texturePathRight2 != "" && rightDist > 60.0f)
            {
                std::string tempTextPath = texturePathRight;
                texturePathRight = texturePathRight2;
                texturePathRight2 = tempTextPath;
                rightDist = 0.0f;
            }
        }
    }
    else if(transform.position.y != rigidBody.previousPosition.y)
    {
        if(transform.position.y > rigidBody.previousPosition.y && positionFacing != "UP")
        {
            if(texturePathUp != "")
            {
                texture.Delete();
                texture.Create(texturePathUp);
                positionFacing = "UP";
            }
        }
        if(transform.position.y < rigidBody.previousPosition.y && positionFacing != "DOWN")
        {
            if(texturePath != "")
            {
                texture.Delete();
                texture.Create(texturePath);
                positionFacing = "DOWN";
            }
        }
    }
}

void Player::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if (collidedObj->name.find("enemy") != std::string::npos && !hit)
    {
        Hit(collisionNormal, dt);
        inBattle = true;
        enemyFighting = collidedObj->name;
    }
    else if(collidedObj->name.find("npc") != std::string::npos)
    {
        npcTalkingTo = collidedObj->name;
    }
}

void Player::Render(Renderer& renderer, const Camera& camera)
{
    if(shaderName == "textureShader"){
        renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
        texture.Unbind();
    }else{
        renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    }
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
    items["sword"]->transform = swordTransform;
}
