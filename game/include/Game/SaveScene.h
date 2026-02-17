#pragma once

class Scene;
class Menu;
class SaveMenu;
class Level;
class Battle;
class MenuItem;
class StringText;

class SaveScene : public Scene {

public:
    SaveScene(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveLevelFilePath, std::string saveBattleFilePath, bool save, std::shared_ptr<Level> loadGame, std::shared_ptr<Battle> loadBattle, std::string root);
    ~SaveScene();

    void Init() override;

    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;

    void LoadSaveScene();
    void GetSavedGames();
    void SaveGame();
    void LoadGame();

    void SetNextScene(std::string nextScene){this->nextScene = nextScene;}
    std::string NextScene(){return nextScene;}

private:
    std::string saveFilePath;
    std::string filepath;
    std::string saveLevelFilePath;
    std::string saveBattleFilePath;
    std::shared_ptr<SaveMenu> menu;
    std::shared_ptr<StringText> title;
    std::shared_ptr<StringText> studio;
    bool save;
    std::shared_ptr<Level> loadGame;
    std::shared_ptr<Battle> loadBattle;
    std::string nextScene;

    std::string root;

};