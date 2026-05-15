#include "Zombie_Run.h"
#include "Zombie_Walk.h"
#include "GameInstance.h"
#include "Zombie_Idle.h"
#include "Zombie_Attack.h"

CZombie_Walk::CZombie_Walk()
{
}

CZombie_Walk::~CZombie_Walk()
{
}

void CZombie_Walk::Enter(CBody_Zombie& owner)
{
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::WALK_FAST) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::WALK_FAST);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::PATROL_WALK) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::PATROL_WALK);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK);
    }
}

void CZombie_Walk::Update(CBody_Zombie& owner, _float deltaTime)
{

}

void CZombie_Walk::Exit(CBody_Zombie& owner)
{

}
unique_ptr<CZombie_Walk> CZombie_Walk::Create()
{
    auto pInstance = unique_ptr<CZombie_Walk>(new CZombie_Walk());

    return pInstance;
}