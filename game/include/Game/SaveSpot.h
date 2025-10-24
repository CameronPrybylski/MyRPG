#include <Engine/Scene/GameObject.h>

class SaveSpot : public GameObject {

public:
    SaveSpot(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~SaveSpot();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    bool GetSaveGame(){return this->saveGame;}
    void SetSaveGame(bool saveGame){this->saveGame = saveGame;}

private:
    bool saveGame;


};