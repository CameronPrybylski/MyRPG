#include <Engine/Scene/GameObject.h>

class TownSpot : public GameObject {

public:
    TownSpot(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~TownSpot();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void SetEnterTown(bool enterTown){this->enterTown = enterTown;}
    bool EnterTown(){return enterTown;}


private:
    bool enterTown;

};