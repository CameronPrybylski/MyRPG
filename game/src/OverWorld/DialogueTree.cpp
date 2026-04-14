#include <Game/DialogueTree.h>

DialogueTree::DialogueTree(std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree) : dialogueTree(dialogueTree)
{
}

DialogueTree::~DialogueTree()
{
}

std::unordered_map<std::string, std::vector<std::string>> DialogueTree::GetNextDialogueOptions(std::string dialogueOption)
{
    for(int i = currentDialoguePos + 1; i < dialogueTree.size(); i++)
    {
        for(auto dialogueOption = dialogueTree[i].begin(); dialogueOption != dialogueTree[i].end(); dialogueOption++)
        {
            if(dialogueOption->first.find(currentDialogueOption) != std::string::npos)
            {
                currentDialoguePos = i;
                return dialogueTree[i];
            }
        }
    }
    std::unordered_map<std::string, std::vector<std::string>> none;
    return none;
}