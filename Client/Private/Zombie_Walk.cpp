#include "Zombie_Run.h"
#include "Zombie_Walk.h"
#include "GameInstance.h"
#include "Zombie_Idle.h"
#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Fatal.h"
#include "Zombie_StealthDeath.h"

CZombie_Walk::CZombie_Walk()
{
}

CZombie_Walk::~CZombie_Walk()
{
}

void CZombie_Walk::Enter(CBody_Zombie& owner)
{
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::WALK_FAST) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::WALK_FAST),0.4f);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::PATROL_WALK) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::PATROL_WALK);
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::PATROL_WALK));
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK);
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK));
    }
}

void CZombie_Walk::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_Damaged()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Damaged::Create());
        return;
    }
    if (owner.Get_PlayerDetected() && !owner.Is_UsingStairs() && !owner.Get_Rotating() && !detectedAnimStart) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1),0.2f, false);
        detectedAnimStart = true;
        animStart = true;
    
    }
    if (detectedAnimStart && !animStart) {
        owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
        return;
    }

    if(!owner.Get_PlayerDetected()) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::WALK_FAST) {
            owner.Get_Transform()->Go_Straight(deltaTime * 0.7f);
        }
        else {
            owner.Get_Transform()->Go_Straight(deltaTime * 0.5f);

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
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }

}

void CZombie_Walk::Exit(CBody_Zombie& owner)
{

}
unique_ptr<CZombie_Walk> CZombie_Walk::Create()
{
    auto pInstance = unique_ptr<CZombie_Walk>(new CZombie_Walk());

    return pInstance;
}