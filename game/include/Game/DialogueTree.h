#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class DialogueTree
{
public:
    DialogueTree(std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree);
    ~DialogueTree();

    std::vector<std::unordered_map<std::string, std::vector<std::string>>> GetDialogueTree(){return this->dialogueTree;}

    std::unordered_map<std::string, std::vector<std::string>> GetNextDialogueOptions(std::string dialogueOption);
    
    void SetCurrentDialoguePos(int curDiaPos){this->currentDialoguePos = curDiaPos;}
    
    void SetCurrentDialogueOption(std::string dialougeOption){this->currentDialogueOption = dialougeOption;}
    std::string GetCurrentDialogueOption(){return this->currentDialogueOption;}

    std::unordered_map<std::string, std::vector<std::string>> GetLeftChild(int index){return dialogueTree[2*index + 1];}
    std::unordered_map<std::string, std::vector<std::string>> GetRightChild(int index){return dialogueTree[2*index + 2];}


private:
    std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree;
    int currentDialoguePos = 0;
    std::string currentDialogueOption;
};