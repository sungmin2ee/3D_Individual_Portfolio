#include "Player_StealthAttack.h"
#include "GameInstance.h"
#include "Player_Idle.h"

CPlayer_StealthAttack::CPlayer_StealthAttack()
{
}

CPlayer_StealthAttack::~CPlayer_StealthAttack()
{
}

void CPlayer_StealthAttack::Enter(CBody_Player& owner)
{
    CBody_Player::PLAYER_STATE a = owner.Get_CurState();
    if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STEALTH_KILL), 0.5f,false);

    }
    else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
        srand((unsigned int)time(0));
        uint32_t random = rand() % 2;
        if (random == 0) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::AXE_STEALTH1), 0.5f, false);
        }
        else {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::AXE_STEALTH2), 0.5f, false);

        }

    }
    owner.Set_MakingSound(false);
    animStart = true;

}

void CPlayer_StealthAttack::Update(CBody_Player& owner, _float deltaTime)
{

    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    if (!animStart) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
    }
}

void CPlayer_StealthAttack::Exit(CBody_Player& owner)
{

}
unique_ptr<CPlayer_StealthAttack> CPlayer_StealthAttack::Create()
{
    auto pInstance = unique_ptr<CPlayer_StealthAttack>(new CPlayer_StealthAttack());

    return pInstance;
}