#include <Game/PlayerInBattle.h>
#include <Game/ConsumableItem.h>
#include <Game/Spell.h>
#include <thread>
#include <chrono>

PlayerInBattle::PlayerInBattle(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic)
{
    mesh = AssetManager::GetMesh("quadMesh");
    shaderName = "textureShader";
    transform.position = position;
    transform.scale = scale;
    deathScale = {scale.y, scale.x, scale.z};
    rigidBody.isStatic = isStatic;
    hp = 15;
    maxHP = hp;
    strength = 1;
    magicLevel = 1;
    mp = 10;
    maxMP = mp;
    level = 1;
    xp = 0;
    xpNeeded = 50;
    xpIncrement = 10;
    this->texturePath = texturePath;
    texture.Create(texturePath);
    this->color = color;
    this->name = name;

    equippedWeapon = std::make_shared<Weapon>(3,"Short Sword");
    weapons["Short Sword"] = equippedWeapon;
    weapons["Bow"] = std::make_shared<Weapon>(3, "Bow");
}

PlayerInBattle::~PlayerInBattle()
{

}

void PlayerInBattle::OnEvent(const Input& input)
{
    
}

void PlayerInBattle::Update(const Input& input, float dt)
{
    if(playerMove == "Attack")
    {
        usingSword = true;
        transform.position.x -= 100.0f;
    }else if(playerMove == "Magic")
    {
        usingSword = false;
        transform.position.x += 100.0f;
    }

    if(hp <= 0)
    {
        alive = false;
        timeOfDeath += 1.0f;
    }

    DeathAnimation();
    RenderMove();
    DamageFlicker();
}

void PlayerInBattle::MakeMove(std::string menuPlayerMove)
{
    ogPosition = transform.position;
    move = true;
    moveForward = true;
    this->totalPlayerMove = menuPlayerMove;
}

void PlayerInBattle::RenderMove()
{
    if(move)
    {
        frame += 1.0f;
        if(totalPlayerMove.find("Attack") != std::string::npos && totalPlayerMove.find("Magic") == std::string::npos)
        {
            if(frame <= 15.0f)
            {
                glm::vec3 newPos(-5.0f, 0.0f, 0.0f);
                transform.position += newPos;
                texture.Delete();
                texture.Create(moveTexturePath);
            }
            else if(frame > 30.0f && frame <= 45.0f)
            {
                glm::vec3 newPos(5.0f, 0.0f, 0.0f);
                transform.position += newPos;
                texture.Delete();
                texture.Create(moveTexturePath);
            }
            else if(frame > 45.0f)
            {
                transform.position = ogPosition;
            }
        }
        if(frame > 15.0f && frame <= 30.0f)
        {
            attack = true;
            if(totalPlayerMove.find("Magic") != std::string::npos)
            {
                texture.Delete();
                texture.Create(magicTexturePath);
            }
            else if(totalPlayerMove.find("Attack") != std::string::npos)
            {
                texture.Delete();
                texture.Create(attackTexturePath);
            }
        }
        else if(frame > 30.0f && frame <= 45.0f)
        {
            moveBackward = true;
            moveForward = false;
            attack = false;
        }
        else if(frame > 45.0f)
        {
            frame = 0.0f;
            move = false;
            moveBackward = false;
            texture.Delete();
            texture.Create(texturePath);
        }
    }
}

void PlayerInBattle::DeathAnimation()
{
    if(deathInProgress && !deathOver)
    {
        if(timeOfDeath == 1.0f)
        {
            texture.Delete();
            texture.Create(deathTexture1Path);
        }
        else if(timeOfDeath == 60.0f)
        {
            transform.scale = deathScale;
            texture.Delete();
            texture.Create(deathTexture2Path);
        }
        else if(timeOfDeath > 120.0f)
        {
            deathOver = true;
        }
    }
}

void PlayerInBattle::TakeDamage(int damage)
{
    hp -= damage;
    takingDamage = true;
}

void PlayerInBattle::DamageFlicker()
{
    if(takingDamage)
    {
        frame += 1.0f;
        if(frame <= 60.0f && (int)frame % 2 == 0)
        {
            texture.Delete();
            texture.Create(damageTexturePath);
        }
        else if(frame <= 60.0f && (int)frame % 2 != 0)
        {
            texture.Delete();
            texture.Create(texturePath);
        }
        else
        {
            texture.Delete();
            texture.Create(texturePath);
            frame = 0.0f;
            takingDamage = false;
        }
    }
}

void PlayerInBattle::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if (collidedObj->name.find("enemy") != std::string::npos && !hit)
    {
        Hit(collisionNormal, dt);
        inBattle = true;
    }
}

void PlayerInBattle::Render(Renderer& renderer, const Camera& camera)
{
    renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    texture.Unbind();
    //std::this_thread::sleep_for(std::chrono::seconds(2));
}

void PlayerInBattle::Hit(glm::vec2 collisionNormal, float dt)
{
    //Collided from Left
    if(collisionNormal.x == 1 || collisionNormal.x == -1)
    {
        rigidBody.velocity.x *= -1;
    }
    else
    {
        rigidBody.velocity.y *= -1;
    }
    hit = true;
    timeSinceHit = 0.0f;
    hp--;
}

void PlayerInBattle::AddItem(std::string name, std::shared_ptr<GameObject> item)
{
    items[name].push_back(item);
}

void PlayerInBattle::AddConsumableItem(std::string name, std::shared_ptr<ConsumableItem> item)
{
    consumableItems[name].push_back(item);
}

void PlayerInBattle::AddWeapon(std::string name, std::shared_ptr<Weapon> weapon)
{
    weapons[name] = weapon;
}

void PlayerInBattle::ChangeWeapon(std::string weaponName)
{
    if(weapons.count(weaponName))
    {
        equippedWeapon = weapons[weaponName];
    }
}

void PlayerInBattle::AddSpell(std::string name, std::shared_ptr<Spell> spell)
{
    spells[name] = spell;
}

void PlayerInBattle::PositionSword()
{
    Transform swordTransform = items["sword"][0]->transform;
    swordTransform.position = transform.position;
    float swordScale = swordTransform.scale.x;
    if(usingSword)
    {
        if(positionFacing == "Down" || positionFacing == "Left")
        {
            swordScale *= -1;
        }
        if(positionFacing == "Right" || positionFacing == "Left")
        {
            swordTransform.position.x = transform.position.x + swordScale;
            if(positionFacing == "Right")
            {
                swordTransform.rotation.z = 180.0f;
            }
            else
            {
                swordTransform.rotation.z = 0.0f;
            }
        }
        else if(positionFacing == "Up" || positionFacing == "Down")
        {
            swordTransform.position.y = transform.position.y + swordScale;
            if(positionFacing == "Up")
            {
                swordTransform.rotation.z = 270.0f;
            }
            else
            {
                swordTransform.rotation.z = 90.0f;
            }
        }
    }
    items["sword"][0]->transform = swordTransform;
}

void PlayerInBattle::CheckXP()
{
    if(xp >= xpNeeded)
    {
        level++;
        strength++;
        magicLevel++;
        double hpRatio = (double)hp / (double)maxHP;
        maxHP += 10;
        hp = hpRatio * maxHP;
        double mpRatio = (double)mp / (double)maxMP;
        maxMP += 5;
        mp = mpRatio * maxMP;
        if(mp <= 0)
        {
            mp = 0.5 * (double)maxMP;
        }
        xpNeeded += (level * xpIncrement);
    }
}

void PlayerInBattle::UseItem(std::string playerMove)
{
    std::string UseItemStr = "UseItem";
    std::string itemUse = playerMove.substr(UseItemStr.length(), playerMove.length() - UseItemStr.length());
    if(!consumableItems[itemUse].empty())
    {
        std::pair<std::string, int> itemEffect = consumableItems[itemUse][0]->UseItem();
        consumableItems[itemUse].pop_back();
        if(itemEffect.first == "hp")
        {
            if(hp + itemEffect.second < maxHP)
            {
                hp += itemEffect.second;
            }
            else
            {
                hp = maxHP;
            }
        }
        else if(itemEffect.first == "mp")
        {
            if(mp + itemEffect.second < maxMP)
            {
                mp += itemEffect.second;
            }
            else
            {
                mp = maxMP;
            }
        }
    }
}

int PlayerInBattle::GetMagicDamage(std::string magicType)
{
    for(auto itr = spells.begin(); itr != spells.end(); itr++)
    {
        if(magicType.find(itr->first) != std::string::npos)
        {
            if(mp >= itr->second->GetMPCost())
            {
                mp -= itr->second->GetMPCost();
                return magicLevel * itr->second->GetDamage();
            }
        }
    }
    return 0;
}

void PlayerInBattle::UseMagic(std::string magicType)
{
    if(spells.count(magicType))
    {
        std::shared_ptr<Spell> spell = spells.at(magicType);
        if(magicType.find("Cur") != std::string::npos && spell->GetMPCost() <= mp)
        {
            if(hp + spell->GetDamage() <= maxHP)
            {
                hp += spell->GetDamage();
            }
            else
            {
                hp = maxHP;
            }
            mp -= spell->GetMPCost();
        }
    }
}