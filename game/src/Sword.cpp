#include <Game/Sword.h>

Sword::Sword(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    rigidBody.velocity = velocity;
    rigidBody.isStatic = isStatic;
    this->color = color;
    this->name = name;
    usingSword = false;
    length = scale.x;
    width = scale.y;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

Sword::~Sword()
{

}

void Sword::OnEvent(const Input &input)
{
    if(input.IsKeyDown("L"))
    {
        usingSword = true;
        transform.scale.x = length;
        transform.scale.y = width;
    }
    else
    {
        usingSword = false;
        transform.scale.x = 5.0f;
        transform.scale.y = 5.0f;
    }
}

void Sword::Update(const Input& input, float dt)
{
    /*
    if(usingSword)
    {
        transform.position.y = player->transform.position.y;
        transform.position.x = player->transform.position.x + transform.scale.x;
    }
    else
    {
        transform.position.y = player->transform.position.y;
        transform.position.x = player->transform.position.x;
    }
    */
}

void Sword::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if(!collidedObj->rigidBody.isStatic && collidedObj->name == "player")
    {
        //collidedObj->rigidBody.velocity.x = rigidBody.velocity.x;
    }

    if(collisionNormal.x == 1 && collisionNormal.y == 0)
    {
        //rigidBody.velocity.x = abs(velocity.x);
    }
    if(collisionNormal.x == -1 && collisionNormal.y == 0)
    {
        //rigidBody.velocity.x = -1 * abs(velocity.x);
    }
}

void Sword::Render(Renderer &renderer, const Camera& camera)
{
    if(usingSword)
    {
        if(shaderName == "textureShader"){
            renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
        }else{
            renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
        }
    }
}