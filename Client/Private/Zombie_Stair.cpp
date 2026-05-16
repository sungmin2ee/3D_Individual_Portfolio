#include "Zombie_Stair.h"
#include "Zombie_Run.h"
#include "Zombie_Walk.h"
#include "GameInstance.h"
#include "Zombie_Idle.h"
#include "Zombie_Attack.h"

CZombie_Stair::CZombie_Stair()
{
}

CZombie_Stair::~CZombie_Stair()
{
}

void CZombie_Stair::Enter(CBody_Zombie& owner)
{
    int a = ETOUI(owner.Get_CurState());
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER), false);
        animStart = true;
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER), false);
        animStart = true;
    }
}

void CZombie_Stair::Update(CBody_Zombie& owner, _float deltaTime)
{

    

}

void CZombie_Stair::Exit(CBody_Zombie& owner)
{
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_EXIT), false);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_EXIT), false);
    }
}
unique_ptr<CZombie_Stair> CZombie_Stair::Create()
{
    auto pInstance = unique_ptr<CZombie_Stair>(new CZombie_Stair());

    return pInstance;
}