#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Run.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Player.h"
#include "Zombie_Fatal.h"

CZombie_Attack::CZombie_Attack()
{
}

CZombie_Attack::~CZombie_Attack()
{
}

void CZombie_Attack::Enter(CBody_Zombie& owner)
{
    owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::ATTACK1),1.4f, false);
    owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::ATTACK1);
    m_iCount++;
    animStart = true;
    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    if (layer == nullptr)
        return;
    auto player = layer->GetObjectFirst();
    if (player == nullptr)
        return;
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
    if (playerBody == nullptr)
        return;
    if (!playerBody->Get_OnHit()) {
        if (owner.Get_Obb()->myOBB.Intersects(playerBody->Get_Obb()->myOBB))
            playerBody->Set_OnHit(true);
    }
    auto a = CGameInstance::Get().Random(0.f, 3.f);
    if (a < 3 && a > 2) {
        CGameInstance::Get().PlaySoundOne(L"zombieAttack1.wav", owner.Get_RunChannelPtr(), 1.0f);

    }
    else if(a < 2 && a > 3) {
        CGameInstance::Get().PlaySoundOne(L"zombieAttack2.wav", owner.Get_RunChannelPtr(), 1.0f);

    }
    else {
        CGameInstance::Get().PlaySoundOne(L"zombieAttack3.wav", owner.Get_RunChannelPtr(), 1.0f);

    }

}

void CZombie_Attack::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    if (owner.Get_Damaged()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Damaged::Create());
        return;
    }

    if (!animStart && !owner.Get_Damaged() && owner.Get_PlayerInRange()) {
        if (m_iCount % 3 == 0) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::ATTACK2), 1.4f, false);
        }
        else if (m_iCount % 3 == 1) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::ATTACK3), 1.4f, false);
        }
        else if (m_iCount % 3 == 2) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::ATTACK1), 1.4f, false);
        }
        auto a = CGameInstance::Get().Random(0.f, 3.f);
        if (a < 3 && a > 2) {
            CGameInstance::Get().PlaySoundOne(L"zombieAttack1.wav", owner.Get_RunChannelPtr(), 1.0f);

        }
        else if (a < 2 && a > 3) {
            CGameInstance::Get().PlaySoundOne(L"zombieAttack2.wav", owner.Get_RunChannelPtr(), 1.0f);

        }
        else {
            CGameInstance::Get().PlaySoundOne(L"zombieAttack3.wav", owner.Get_RunChannelPtr(), 1.0f);

        }
        animStart = true;
        m_iCount++;
        auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
        if (layer == nullptr)
            return;
        auto player = layer->GetObjectFirst();
        if (player == nullptr)
            return;
        auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
        if (playerBody == nullptr)
            return;
        playerBody->Set_OnHit(true);
        return;
    
    }
     
     
    if (!animStart && owner.Get_PlayerDetected()&& !owner.Get_Damaged()) {
        if (!owner.Get_Damaged() && !owner.Get_PlayerInRange()) {
            owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
            owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::RUN);
            return;
        }
    }
    if (owner.Get_HP() <= 21) {
        owner.Get_StateMachine()->ChangeState(CZombie_Fatal::Create());
        return;
    }
  



    
    //if()

   



}

void CZombie_Attack::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Attack> CZombie_Attack::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CZombie_Attack>(new CZombie_Attack());

    return pInstance;
}