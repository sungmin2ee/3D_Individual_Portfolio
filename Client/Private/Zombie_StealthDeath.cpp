#include "Zombie_StealthDeath.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Body_Player.h"
#include "Player.h"

CZombie_StealthDeath::CZombie_StealthDeath()
{
}

CZombie_StealthDeath::~CZombie_StealthDeath()
{
}

void CZombie_StealthDeath::Enter(CBody_Zombie& owner)
{
    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    if (layer == nullptr)
        return;
    auto player = layer->GetObjectFirst();
    if (player == nullptr)
        return;
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
    if (playerBody == nullptr)
        return;
    if (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::AXE_STEALTH1)) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AXE_STEALTH1),false);
    }
    else if (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::AXE_STEALTH2)) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AXE_STEALTH2), false);
    }
    else if (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::STEALTH_KILL)) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::UNARMED_STEALTH), false);
    }
    auto playerPos = playerBody->Get_Transform()->Get_State(STATE::POSITION);
    _vector delta = XMVectorSet(0.13f, 0, 0, 0);
    if (playerBody->Get_CurDir() == CBody_Player::PLAYER_DIR::RIGHT) {
        playerPos += delta;
    }
    else if (playerBody->Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT) {
        playerPos -= delta;

    }
    owner.Get_Transform()->Set_State(STATE::POSITION, playerPos);
    auto pos = owner.Get_Transform()->Get_State(STATE::POSITION);
    owner.Set_HPZero();

}

void CZombie_StealthDeath::Update(CBody_Zombie& owner, _float deltaTime)
{
  
    auto pos = owner.Get_Transform()->Get_State(STATE::POSITION);
    
}

void CZombie_StealthDeath::Exit(CBody_Zombie& owner)
{

}
unique_ptr<CZombie_StealthDeath> CZombie_StealthDeath::Create()
{
    auto pInstance = unique_ptr<CZombie_StealthDeath>(new CZombie_StealthDeath());

    return pInstance;
}