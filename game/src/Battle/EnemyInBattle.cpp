#include <Game/EnemyInBattle.h>
#include <random>
#include <chrono>

EnemyInBattle::EnemyInBattle(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    this->color = color;
    this->isAlive = true;
    this->name = name;
    this->type = "Enemy";
    hp = 5;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

EnemyInBattle::~EnemyInBattle()
{
}

void EnemyInBattle::OnEvent(const Input &input)
{
}

void EnemyInBattle::Update(float dt)
{
}

void EnemyInBattle::Update(const Input &input, float dt)
{
    if(enemyMove)
    {
        MakeMove();
    }
}

void EnemyInBattle::MakeMove()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 2. Create a random number engine (e.g., Mersenne Twister)
    std::mt19937 generator(seed);
    // 3. Define a uniform integer distribution for the desired range [0, 1]
    std::uniform_int_distribution<int> distribution(0, 1);
    // 4. Generate the random integer
    int random_value = distribution(generator);
    if(random_value == 0)
        transform.position.x +=  100.0f;
    else
        transform.position.x -= 100.0f;
}

void EnemyInBattle::TakeDamage(int damage)
{
    //Why is this 1 should be 2 or whatever damage was set to
    hp -= damage;
}

void EnemyInBattle::Render(Renderer &renderer, const Camera &camera)
{
    renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader("objectShader"), color);
}

void EnemyInBattle::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}
