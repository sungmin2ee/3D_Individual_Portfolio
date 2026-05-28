#include "Zombie_Fatal.h"
#include "GameInstance.h"
#include "Layer.h"


CZombie_Fatal::CZombie_Fatal()
{
}

CZombie_Fatal::~CZombie_Fatal()
{
}

void CZombie_Fatal::Enter(CBody_Zombie& owner)
{
    owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::KNOCKDOWN), 0.7f, false);
    owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::KNOCKDOWN);

}

void CZombie_Fatal::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    if (!animStart&& !m_bDead) {
        if (owner.Get_PlayerInRange() && owner.Is_Executing()) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::UNARMED_GROUND_EXECUTE), 0.7f, false);
            animStart = true;
            m_bDead = true;
            owner.Set_HPZero();
            return;
        }
    }
    
}

void CZombie_Fatal::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Fatal> CZombie_Fatal::Create()
{
    auto pInstance = unique_ptr<CZombie_Fatal>(new CZombie_Fatal());

    return pInstance;
}