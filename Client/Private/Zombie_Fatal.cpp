#include "Zombie_Fatal.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Zombie.h"
#include "Player.h"
#include "Body_Player.h"
#include "Blood.h"


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
    if (m_bDead && !animFinished) {
        animTimer += deltaTime;
        auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
        auto player = layer->GetObjectFirst();
        auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();

        if (animTimer > 1.0f && (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::GROUND_EXECUTE1))) {
            animFinished = true;
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
        }
        if (animTimer > 0.7f && (playerBody->Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::GROUND_EXECUTE2))) {
            animFinished = true;
            auto zombie = static_pointer_cast<CZombie>(owner.Get_Zombie().lock());
            static_cast<CBlood*>(zombie->Get_Effect())->Play_Particle();
            static_cast<CBlood*>(zombie->Get_Effect())->Set_Bleeding();

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