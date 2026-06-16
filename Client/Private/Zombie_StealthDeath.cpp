#include "Zombie_StealthDeath.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Body_Player.h"
#include "Player.h"
#include "Zombie.h"
#include "Blood.h"

CZombie_StealthDeath::CZombie_StealthDeath()
{
}

CZombie_StealthDeath::~CZombie_StealthDeath()
{
}

void CZombie_StealthDeath::Enter(CBody_Zombie& owner)
{
    CGameInstance::Get().PlaySoundOne(L"zombieWalk.wav", owner.Get_RunChannelPtr(), 1.0f);

    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    if (layer == nullptr)
        return;
    auto player = layer->GetObjectFirst();
    if (player == nullptr)
        return;
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
    if (playerBody == nullptr)
        return;
    _vector delta;
    if (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::AXE_STEALTH1)) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AXE_STEALTH1),false);
        delta = XMVectorSet(0.1f, 0, 0, 0);

    }
    else if (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::AXE_STEALTH2)) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AXE_STEALTH2), false);
        delta = XMVectorSet(0.12f, 0, 0, 0);
        CGameInstance::Get().PlaySoundOne(L"execute.wav", owner.Get_RunChannelPtr(), 1.0f);

    }
    else if (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::STEALTH_KILL)) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::UNARMED_STEALTH), false);
        delta = XMVectorSet(0.13f, 0, 0, 0);

    }
    auto playerPos = player->Get_Transform()->Get_State(STATE::POSITION);
    if (playerBody->Get_CurDir() == CBody_Player::PLAYER_DIR::RIGHT) {
        playerPos += delta;
    }
    else if (playerBody->Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT) {
        playerPos -= delta;
    
    }
    owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, playerPos);
    auto pos = owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION);
    owner.Set_HPZero();

}

void CZombie_StealthDeath::Update(CBody_Zombie& owner, _float deltaTime)
{
    owner.Get_Model()->Play_Animation(deltaTime);


    animtime += deltaTime;
    if (owner.Get_Model()->Get_AnimIndex() == ETOUI(CBody_Zombie::ZOMBIE_STATE::UNARMED_STEALTH) && !animFinshed) {
        if (animtime > 1.8f && count == 0) {
            CGameInstance::Get().PlaySoundOne(L"execute.wav", owner.Get_RunChannelPtr(), 1.0f);
            count++;
        }
        else if (animtime > 2.3f && count == 1) {
            CGameInstance::Get().PlaySoundOne(L"execute.wav", owner.Get_RunChannelPtr(), 1.0f);
            count++;

        }else if (animtime > 3.2f && count == 2) {
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();
            CGameInstance::Get().PlaySoundOne(L"bloodSound.wav", owner.Get_RunChannelPtr(), 1.0f);
            animFinshed = true;
        }
    }
    if (owner.Get_Model()->Get_AnimIndex() == ETOUI(CBody_Zombie::ZOMBIE_STATE::AXE_STEALTH1)) {
        if (animtime > 0.5f && count == 0 && !animFinshed) {
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();
            CGameInstance::Get().PlaySoundOne(L"execute.wav", owner.Get_RunChannelPtr(), 1.0f);

            count++;
        }
        if (animtime > 1.8f && count == 1 && !animFinshed) {
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
            CGameInstance::Get().PlaySoundOne(L"execute.wav", owner.Get_RunChannelPtr(), 1.0f);

            count++;
        }
        if (animtime > 3.f && count == 2 && !animFinshed) {
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();
            CGameInstance::Get().PlaySoundOne(L"bloodSound.wav", owner.Get_RunChannelPtr(), 1.0f);
            animFinshed = true;
        }
        
    }
    if (owner.Get_Model()->Get_AnimIndex() == ETOUI(CBody_Zombie::ZOMBIE_STATE::AXE_STEALTH2)) {
        if (animtime > 1.5f && !animFinshed) {
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
            CGameInstance::Get().PlaySoundOne(L"bloodSound.wav", owner.Get_RunChannelPtr(), 1.0f);
            animFinshed = true;
        }
    }

   // auto pos = owner.Get_Transform()->Get_State(STATE::POSITION);
    
}

void CZombie_StealthDeath::Exit(CBody_Zombie& owner)
{

}
unique_ptr<CZombie_StealthDeath> CZombie_StealthDeath::Create()
{
    auto pInstance = unique_ptr<CZombie_StealthDeath>(new CZombie_StealthDeath());

    return pInstance;
}