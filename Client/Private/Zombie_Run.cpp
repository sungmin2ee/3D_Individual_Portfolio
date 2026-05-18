#include "Zombie_Run.h"
#include "GameInstance.h"
#include "Zombie_Idle.h"
#include "Zombie_Walk.h"
#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Fatal.h"

CZombie_Run::CZombie_Run()
{
}

CZombie_Run::~CZombie_Run()
{
}

void CZombie_Run::Enter(CBody_Zombie& owner)
{
    owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::RUN), 0.7f);
}

void CZombie_Run::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    //CHASE PLALYER
    if (owner.Get_Damaged()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Damaged::Create());
        return;
    }
    if (owner.Get_PlayerInRange()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Attack::Create());
        return;
    }
    owner.Get_Transform()->Go_Straight(deltaTime * 3.f);

    if (owner.Get_HP() <= 21) {
        owner.Get_StateMachine()->ChangeState(CZombie_Fatal::Create());
        return;
    }

}

void CZombie_Run::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Run> CZombie_Run::Create()
{
    auto pInstance = unique_ptr<CZombie_Run>(new CZombie_Run());

    return pInstance;
}