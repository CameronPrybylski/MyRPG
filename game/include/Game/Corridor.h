#pragma once

#include <Game/Obstacle.h>

#include <set>

class Corridor : public Obstacle
{
public:
    Corridor(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, std::set<int> gaps);
    ~Corridor();

    void SetWalls();
    std::vector<std::shared_ptr<Obstacle>> GetWalls(){return this->walls;}


private:
    std::vector<std::shared_ptr<Obstacle>> walls;
    std::set<int> gaps;
};