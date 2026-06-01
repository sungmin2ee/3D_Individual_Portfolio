#include "Player_Execute.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Idle.h"
#include "Player_Walk.h"

CPlayer_Execute::CPlayer_Execute()
{
}

CPlayer_Execute::~CPlayer_Execute()
{
}

void CPlayer_Execute::Enter(CBody_Player& owner)
{
    srand((unsigned int)time(0));
    uint32_t random = rand() % 2;
    
    if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
        if (random == 0) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::GROUND_EXECUTE1),0.4f, false);
        }
        else {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::GROUND_EXECUTE2), 0.4f, false);

        }
    }

    animStart = true;
}

void CPlayer_Execute::Update(CBody_Player& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }


    if (!animStart) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
    }




}

void CPlayer_Execute::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Execute> CPlayer_Execute::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Execute>(new CPlayer_Execute());

    return pInstance;
}