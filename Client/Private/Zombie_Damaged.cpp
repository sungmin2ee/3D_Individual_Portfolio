#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Run.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Player.h"
#include "Zombie_Fatal.h"

CZombie_Damaged::CZombie_Damaged()
{
}

CZombie_Damaged::~CZombie_Damaged()
{
}

void CZombie_Damaged::Enter(CBody_Zombie& owner)
{
    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    auto player = layer->GetObjectFirst();
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
    if (playerBody->Get_Model()->Get_AnimIndex() == 31 || playerBody->Get_Model()->Get_AnimIndex() == 2) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1),1.5f, false);
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1);
    }else if (playerBody->Get_Model()->Get_AnimIndex() == 32 || playerBody->Get_Model()->Get_AnimIndex() == 3) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT2), 1.5f, false);
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::HIT_REACT2);
    }
    owner.Set_Damaged();
    if (playerBody->Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
        owner.Set_HP(-20);

    }
    else  if (playerBody->Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
        owner.Set_HP(-30);
    }
    m_iCount++;
    animStart = true;
    CGameInstance::Get().PlaySoundOne(L"zombieHit.wav", owner.Get_RunChannelPtr(), 1.0f);

}

void CZombie_Damaged::Update(CBody_Zombie& owner, _float deltaTime)
{
    m_fTime += deltaTime;
    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    auto player = layer->GetObjectFirst();
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();


    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {

        animStart = false;
    }
    if (!animStart && owner.Get_Damaged()) {
        if (playerBody->Get_Model()->Get_AnimIndex() == 31 || playerBody->Get_Model()->Get_AnimIndex() == 2) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1), 1.5f, false);
            owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::HIT_REACT1);
            if (playerBody->Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
                owner.Set_HP(-20);

            }
            else  if (playerBody->Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
                owner.Set_HP(-30);
            }
            CGameInstance::Get().PlaySoundOne(L"zombieHit.wav", owner.Get_RunChannelPtr(), 1.0f);


        }
        else if (playerBody->Get_Model()->Get_AnimIndex() == 32 || playerBody->Get_Model()->Get_AnimIndex() == 3) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::HIT_REACT2), 1.5f, false);
            owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::HIT_REACT2);
            if (playerBody->Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
                owner.Set_HP(-20);

            }
            else  if (playerBody->Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
                owner.Set_HP(-30);
            }
            CGameInstance::Get().PlaySoundOne(L"zombieHit.wav", owner.Get_RunChannelPtr(), 1.0f);

        }
        m_iCount++;
        animStart = true;
        m_fTime = 0.f;
    }
    if (!animStart) {
        if (owner.Get_PlayerInRange()) {
            if (!playerBody->Get_OnHit())
                owner.Get_StateMachine()->ChangeState(CZombie_Attack::Create());
            return;
        }
        else {
            //owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
            return;
        }
    }
    //if (m_fTime >= 0.8f) {
    //    if (owner.Get_PlayerInRange()) {
    //        auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    //        auto player = layer->GetObjectFirst();
    //        auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
    //        if(!playerBody->Get_OnHit())
    //            owner.Get_StateMachine()->ChangeState(CZombie_Attack::Create());
    //        return;
    //    }
    //    else {
    //       // owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
    //        return;
    //
    //    }
    //    
    //}
    //
    if (owner.Get_HP() <= 21) {
        owner.Get_StateMachine()->ChangeState(CZombie_Fatal::Create());
        return;
    }

}

void CZombie_Damaged::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Damaged> CZombie_Damaged::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CZombie_Damaged>(new CZombie_Damaged());

    return pInstance;
}