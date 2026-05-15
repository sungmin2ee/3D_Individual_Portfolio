#include "Zombie_Dead.h"
#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Run.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Player.h"

CZombie_Dead::CZombie_Dead()
{
}

CZombie_Dead::~CZombie_Dead()
{
}

void CZombie_Dead::Enter(CBody_Zombie& owner)
{
    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    auto player = layer->GetObjectFirst();
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
  
    //if(playerBody->Get_CurState() == CBody_Player::PLAYER_STATE::)
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1));
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2));
    }
    owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1), false);
    owner.Set_Damaged();
    owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1);
    m_iCount++;
    animStart = true;
}

void CZombie_Dead::Update(CBody_Zombie& owner, _float deltaTime)
{
    m_fTime += deltaTime;

    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    if (!animStart && owner.Get_Damaged()) {
        if (m_iCount % 2 == 0) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT2), false);
        }
        else if (m_iCount % 2 == 1) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1), false);
        }
        m_iCount++;
        animStart = true;
        m_fTime = 0.f;
    }
    if (!owner.Get_Damaged() && m_fTime >= 0.5f) {
        if (owner.Get_PlayerInRange()) {
            owner.Get_StateMachine()->ChangeState(CZombie_Attack::Create());
            return;
        }
        else {
            owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
            return;

        }

    }


}

void CZombie_Dead::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Dead> CZombie_Dead::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CZombie_Dead>(new CZombie_Dead());

    return pInstance;
}