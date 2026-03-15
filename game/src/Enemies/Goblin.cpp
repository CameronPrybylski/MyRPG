#include <Game/Goblin.h>
#include <random>
#include <chrono>

Goblin::Goblin(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, int attackDamage, int xp)
 : EnemyInBattle(position, scale, color, texturePath, name, attackDamage, xp)
{
}

Goblin::~Goblin()
{
}

void Goblin::OnEvent(const Input &input)
{
}

void Goblin::Update(float dt)
{
}

void Goblin::Update(const Input &input, float dt)
{
    if(hp <= 0)
    {
        isAlive = false;
    }
    if(enemyMove && isAlive)
    {
        MakeMove();
        if(transform.position.x - (transform.scale.x / 2) <= 0.0f)
        {
            transform.position.x = (transform.scale.x / 2);
        }
    }
}

void Goblin::MakeMove()
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

void Goblin::TakeDamage(int damage)
{
    //Why is this 1 should be 2 or whatever damage was set to
    hp -= damage;
}

void Goblin::Render(Renderer &renderer, const Camera &camera)
{
    if(isAlive)
    {
        if(shaderName == "textureShader")
        {
            renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
        }
        else
        {
            renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
        }
    }
}

void Goblin::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}
