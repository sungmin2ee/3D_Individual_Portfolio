#include "Zombie_Run.h"
#include "Zombie_Walk.h"
#include "GameInstance.h"
#include "Zombie_Idle.h"
#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Fatal.h"
#include "Zombie_StealthDeath.h"
#include "Zombie.h"

CZombie_Walk::CZombie_Walk()
{
}

CZombie_Walk::~CZombie_Walk()
{
}

void CZombie_Walk::Enter(CBody_Zombie& owner)
{
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::WALK_FAST) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::WALK_FAST),0.8f);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::PATROL_WALK) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::PATROL_WALK);
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::PATROL_WALK),2.f);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK);
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK),2.f);
    }
    CGameInstance::Get().PlaySoundLoop(L"zombieWalk.wav", owner.Get_RunChannelPtr(), 0.0f);

}

void CZombie_Walk::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    if (owner.Get_RunChannel())
    {
        CGameInstance::Get().SetChannelVolume(owner.Get_RunChannelPtr(), owner.Get_Volume());
    }
    if (owner.Get_Damaged()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Damaged::Create());
        return;
    }
    if (owner.Get_PlayerDetected() && !owner.Is_UsingStairs() && !owner.Get_Rotating() && !detectedAnimStart) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1),0.4f, false);
        detectedAnimStart = true;
        animStart = true;
    
    }
    if (detectedAnimStart && !animStart) {
        owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
        return;
    }

    if(!owner.Get_PlayerDetected()) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::WALK_FAST) {
            owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 0.7f);
        }
        else {
            owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 0.5f);

        }
    }
    if (owner.Get_HP() <= 21) {
        owner.Get_StateMachine()->ChangeState(CZombie_Fatal::Create());
        return;
    }
    if (owner.Stealth_Death()) {
        owner.Get_StateMachine()->ChangeState(CZombie_StealthDeath::Create());
        return;
    }
 

}

void CZombie_Walk::Exit(CBody_Zombie& owner)
{
    if (owner.Get_RunChannel())
    {
        CGameInstance::Get().StopSound(owner.Get_RunChannelPtr());
    }
}
unique_ptr<CZombie_Walk> CZombie_Walk::Create()
{
    auto pInstance = unique_ptr<CZombie_Walk>(new CZombie_Walk());

    return pInstance;
}