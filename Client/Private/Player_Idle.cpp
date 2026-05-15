#include "Player_Idle.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Walk.h"
#include "Player_Attack.h"

CPlayer_Idle::CPlayer_Idle()
{
}

CPlayer_Idle::~CPlayer_Idle()
{
}

void CPlayer_Idle::Enter(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND ||
        owner.Get_CurState() == CBody_Player::PLAYER_STATE::END) {
        owner.Get_Model()->Set_Animation(11);
        owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);

    }else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(19);
        owner.Set_CurState(CBody_Player::PLAYER_STATE::SIT);

    }

}

void CPlayer_Idle::Update(CBody_Player& owner, _float deltaTime)
{

    int a = owner.Get_Model()->Get_AnimIndex();
    if (CGameInstance::Get().Key_Pressing(DIK_LEFT) || CGameInstance::Get().Key_Pressing(DIK_RIGHT)) {
        // 만약 W키를 누르면 Run 상태로 변경
         owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
    }
    if (CGameInstance::Get().Key_Down(DIK_F)) {

        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
            owner.Get_Model()->Set_Animation(11);
            animStart = false;
            owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
        }
        else {
            owner.Get_Model()->Set_Animation(20, false);
            animStart = true;
            owner.Set_CurState(CBody_Player::PLAYER_STATE::SIT);
        }
    }

    if (animStart) {
        if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
            owner.Get_Model()->Set_Animation(19);
            animStart = false;
        }
    }
    if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Attack::Create());
    }
}

void CPlayer_Idle::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Idle> CPlayer_Idle::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Idle>(new CPlayer_Idle());

    return pInstance;
}