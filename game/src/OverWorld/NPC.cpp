#include <Game/NPC.h>

NPC::NPC(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, std::vector<std::string> dialogue, float maxDist)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    rigidBody.velocity = velocity;
    this->velocity = velocity;
    rigidBody.isStatic = isStatic;
    this->color = color;
    this->name = name;
    this->type = "NPC";
    this->dialogue = dialogue;
    this->velocity = velocity;
    this->maxDist = maxDist;
    this->texturePath = texturePath;
    this->totalVelocity = std::abs(velocity.x) + std::abs(velocity.y);
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

NPC::~NPC()
{
}

void NPC::OnEvent(const Input &input)
{
}

void NPC::Update(const Input &input, float dt)
{
    glm::vec3 changeDist = abs(rigidBody.previousPosition - transform.position);
    if(changeDist.x != 0)
    {
        distance += changeDist.x;
    }else{
        distance += changeDist.y;
    }
    if(totalVelocity > 0.0f && frame > (3000.0f / totalVelocity) && texturePath != "" && texturePath2 != "")
    {
        std::string texturePathTemp = texturePath;
        texturePath = texturePath2;
        texturePath2 = texturePathTemp;
        texture.Delete();
        texture.Create(texturePath);
        frame = 0.0f;
    }
    if(distance >= maxDist)
    {
        rigidBody.velocity *= -1;
        velocity = rigidBody.velocity;
        distance = 0.0f;
    }
    if(talking)
    {
        rigidBody.velocity = {0.0f, 0.0f, 0.0f};
    }
    else
    {
        rigidBody.velocity = velocity;
    }
    frame += 1.0f;
}

void NPC::Render(Renderer &renderer, const Camera &camera)
{
    if(shaderName == "textureShader"){
        renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    }else{
        renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    }
}

void NPC::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if(collisionNormal.x == 1 || collisionNormal.x == -1)
    {
        if((collisionNormal.x > 0 && rigidBody.velocity.x < 0) || (collisionNormal.x < 0 && rigidBody.velocity.x > 0))
            rigidBody.velocity.x *= -1.0f;
    }
    if(collisionNormal.y == 1 || collisionNormal.y == -1)
    {
        if((collisionNormal.y > 0 && rigidBody.velocity.y < 0) || (collisionNormal.y < 0 && rigidBody.velocity.y > 0))
            rigidBody.velocity.y *= -1.0f;
    }
    if(rigidBody.velocity.x != 0.0f || rigidBody.velocity.y != 0.0f)
    {
        velocity = rigidBody.velocity;
    }
    else
    {
        rigidBody.velocity = -1.0f * velocity;
        velocity = rigidBody.velocity;
    }
}

void NPC::Dialogue()
{

}
