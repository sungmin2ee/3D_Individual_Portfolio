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
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_UP) {
        owner.Get_Model()->Set_Animation(13, false);
        animStart = true;
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_DOWN) {
        owner.Get_Model()->Set_Animation(15, false);
        animStart = true;
    }
}

void CZombie_Stair::Update(CBody_Zombie& owner, _float deltaTime)
{

    if (CGameInstance::Get().Key_Pressing(DIK_LEFT)) {
        if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
            m_eCurstate = DOWNFAST;
        }
        else {
            m_eCurstate = DOWN;
        }
    }
    else if (CGameInstance::Get().Key_Pressing(DIK_RIGHT)) {
        if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
            m_eCurstate = UPFAST;

        }
        else {
            m_eCurstate = UP;
        }
    }
    else if(CGameInstance::Get().Key_Up(DIK_LEFT) || CGameInstance::Get().Key_Up(DIK_RIGHT)){
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_UP) {
            m_eCurstate = IDLE_UP;
        }
        else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_DOWN) {
            m_eCurstate = IDLE_DOWN;
        }
    }

    if (m_ePrestate != m_eCurstate) {
        switch (m_eCurstate) {
        case DOWNFAST:
            owner.Get_Model()->Set_Animation(25);
            break;
        case DOWN:
            owner.Get_Model()->Set_Animation(27);
            break;
        case UPFAST:
            owner.Get_Model()->Set_Animation(26);
            break;
        case UP:
            owner.Get_Model()->Set_Animation(28);
            break;
        case IDLE_UP:
            owner.Get_Model()->Set_Animation(24);
            break;
        case IDLE_DOWN:
            owner.Get_Model()->Set_Animation(23);
            break;
        }
        m_ePrestate = m_eCurstate;

    }

}

void CZombie_Stair::Exit(CBody_Zombie& owner)
{
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_UP) {
        owner.Get_Model()->Set_Animation(14, false);
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_DOWN) {
        owner.Get_Model()->Set_Animation(16, false);
    }
}
unique_ptr<CZombie_Stair> CZombie_Stair::Create()
{
    auto pInstance = unique_ptr<CZombie_Stair>(new CZombie_Stair());

    return pInstance;
}