#include <Game/NPC.h>

NPC::NPC(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, std::vector<std::string> dialogue)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    rigidBody.velocity = velocity;
    rigidBody.isStatic = isStatic;
    this->color = color;
    this->name = name;
    this->type = "NPC";
    this->dialogue = dialogue;
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
}

void NPC::Dialogue()
{

}
