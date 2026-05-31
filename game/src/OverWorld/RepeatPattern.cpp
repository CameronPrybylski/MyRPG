#include <Game/RepeatPattern.h>
#include <Game/Obstacle.h>

RepeatPattern::RepeatPattern(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, glm::vec2 tileScale)
{
    mesh = AssetManager::GetMesh("batchMesh");
    batched = true;
    transform.position = position;
    transform.scale = scale;
    transform.rotation = rotation;
    rigidBody.velocity = velocity;
    rigidBody.isStatic = isStatic;
    this->color = color;
    this->name = name;
    this->texturePath = texturePath;
    int textureNum = texturePaths.size();
    texturePaths.push_back(texturePath);
    Texture texture;
    textures.push_back(texture);
    this->tileScale = tileScale;
    if(texturePath != ""){
        shaderName = "textureBatchShader";
        //texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
    Init();
}

RepeatPattern::~RepeatPattern()
{
}

void RepeatPattern::Init()
{
    /*
     To Do: Implement Batch Rendering 
    */
    int numXObjs = transform.scale.x / tileScale.x;
    int numYObjs = transform.scale.y / tileScale.y;
    float startX = transform.position.x - transform.scale.x / 2;
    float startY = transform.position.y - transform.scale.y / 2;
    glm::vec3 startPos = {startX, startY, transform.position.z};
    std::vector<float> vertices;
    for(int i = 0; i < numYObjs; ++i)
    {
        for(int j = 0; j < numXObjs; ++j)
        {
            /*
            std::shared_ptr<Obstacle> obst = std::make_shared<Obstacle>(startPos, glm::vec3{50.0f,50.0f,0.0f}, 
                transform.rotation, rigidBody.velocity, color, texturePath, name + std::to_string(i), true);
            patterns.push_back(obst);
            startPos.x += 50.0f;
            */
            std::vector<float> quad = 
            {
                startPos.x, startPos.y, 0.0f, 0.0f, 0.0f,
                startPos.x + tileScale.x, startPos.y, 1.0f, 0.0f, 0.0f,
                startPos.x + tileScale.x, startPos.y + tileScale.y, 1.0f, 1.0f, 0.0f,
                startPos.x, startPos.y + tileScale.y, 0.0f, 1.0f, 0.0f
            };
            for(int k = 0; k < quad.size(); ++k)
            {
                vertices.push_back(quad[k]);
            }
            startPos.x += tileScale.x;
        }
        startPos.y += tileScale.y;
        startPos.x = startX;
    }
    mesh->SetVertices(vertices);
    this->vertices = vertices;
}

void RepeatPattern::AddVertices(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, glm::vec2 tileScale)
{
    int numXObjs = scale.x / tileScale.x;
    int numYObjs = scale.y / tileScale.y;
    float startX = position.x - scale.x / 2;
    float startY = position.y - scale.y / 2;
    glm::vec3 startPos = {startX, startY, position.z};
    int textureNum = texturePaths.size();
    for(int i = 0; i < numYObjs; ++i)
    {
        for(int j = 0; j < numXObjs; ++j)
        {
            std::vector<float> quad = 
            {
                startPos.x, startPos.y, 0.0f, 0.0f, (float)textureNum,
                startPos.x + tileScale.x, startPos.y, 1.0f, 0.0f, (float)textureNum,
                startPos.x + tileScale.x, startPos.y + tileScale.y, 1.0f, 1.0f, (float)textureNum,
                startPos.x, startPos.y + tileScale.y, 0.0f, 1.0f, (float)textureNum
            };
            for(int k = 0; k < quad.size(); ++k)
            {
                vertices.push_back(quad[k]);
            }
            startPos.x += tileScale.x;
        }
        startPos.y += tileScale.y;
        startPos.x = startX;
    }
    mesh->SetVertices(vertices);
    texturePaths.push_back(texturePath);
    Texture texture;
    textures.push_back(texture);
}


void RepeatPattern::OnEvent(const Input &input)
{
}

void RepeatPattern::Update(const Input &input, float dt)
{
    if(reverseTexturePath != "")
    {
        ++frame;
        if(frame == 60)
        {
            texture.Delete();
            texture.Create(reverseTexturePath);
            std::string temp = reverseTexturePath;
            reverseTexturePath = texturePath;
            texturePath = temp;
            frame = 0;
        }
    }
}

void RepeatPattern::Render(Renderer &renderer, const Camera &camera)
{
    /*
     To Do: Implement Batch Rendering 
    */
    if(shaderName == "textureBatchShader")
    {
        //std::vector<Texture> textures(texturePaths.size());
        for(int i = 0; i < textures.size(); ++i)
        {
            textures[i].Create(texturePaths[i]);
        }

        renderer.DrawTexturedBatch(*mesh, camera, AssetManager::GetShader(shaderName), textures, color);

        for(int i = 0; i < textures.size(); ++i)
        {
            textures[i].Unbind();
            textures[i].Delete();
        }
    }
    else if(shaderName == "objectShader")
    {
        renderer.DrawBatch(*mesh, camera, AssetManager::GetShader(shaderName), color);
    }
}

void RepeatPattern::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}
