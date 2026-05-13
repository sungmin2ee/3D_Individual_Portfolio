#include "Player_Idle.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Walk.h"

CPlayer_Idle::CPlayer_Idle(_float velocity): State<CBody_Player>(velocity)
{
}

CPlayer_Idle::~CPlayer_Idle()
{
}

void CPlayer_Idle::Enter(CBody_Player& owner)
{
	owner.Get_Model()->Set_Animation(11);
}

void CPlayer_Idle::Update(CBody_Player& owner, _float deltaTime)
{
    if (CGameInstance::Get().Key_Down(DIK_LEFT)) {
        // 만약 W키를 누르면 Run 상태로 변경
         owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
    }
    if (CGameInstance::Get().Key_Down(DIK_F)) {

        if (sitting) {
            owner.Get_Model()->Set_Animation(11);
            animStart = false;
            sitting = false;
            owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
        }
        else {
            owner.Get_Model()->Set_Animation(20, false);
            animStart = true;
            sitting = true;
            owner.Set_CurState(CBody_Player::PLAYER_STATE::SIT);
        }
    }

    if (animStart) {
        if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
            owner.Get_Model()->Set_Animation(19, true);
            animStart = false;
        }
    }
}

void CPlayer_Idle::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Idle> CPlayer_Idle::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Idle>(new CPlayer_Idle(0.f));

    return pInstance;
}