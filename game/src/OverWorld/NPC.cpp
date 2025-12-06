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
        rigidBody.velocity.x *= -1.0f;
    }
    if(collisionNormal.y == 1 || collisionNormal.y == -1)
    {
        rigidBody.velocity.y *= -1.0f;
    }
    if(rigidBody.velocity.x != 0.0f || rigidBody.velocity.y != 0.0f)
    {
        velocity = rigidBody.velocity;
    }
}

void NPC::Dialogue()
{

}
