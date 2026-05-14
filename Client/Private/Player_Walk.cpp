#include "Player_Run.h"
#include "Player_Walk.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Player_Attack.h"

CPlayer_Walk::CPlayer_Walk(_float velocity) : State<CBody_Player>(velocity)
{
}

CPlayer_Walk::~CPlayer_Walk()
{
}

void CPlayer_Walk::Enter(CBody_Player& owner)
{
    int a = ETOUI(owner.Get_CurState());
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(22, true);
    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(33, true);
    }
}

void CPlayer_Walk::Update(CBody_Player& owner, _float deltaTime)
{

  
    if (CGameInstance::Get().Key_Pressing(DIK_LEFT) || CGameInstance::Get().Key_Pressing(DIK_RIGHT)) {
        if (!owner.Get_Rotating()) {
            if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
                owner.Get_Transform()->Go_Straight(deltaTime * 0.7f);
            }
            else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
                owner.Get_Transform()->Go_Straight(deltaTime);
            }
        }
       
    }
  

  
    //if (CGameInstance::Get().Key_Up(DIK_LEFT) || CGameInstance::Get().Key_Up(DIK_RIGHT)) {
    //    owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
    //}
    if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT) && 
        (CGameInstance::Get().Key_Pressing(DIK_LEFT)|| CGameInstance::Get().Key_Pressing(DIK_RIGHT))) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
    }

    if (CGameInstance::Get().Key_Up(DIK_LEFT) || CGameInstance::Get().Key_Up(DIK_RIGHT)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
    }
    if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Attack::Create());
    }
}

void CPlayer_Walk::Exit(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(34, false);
    }
}
unique_ptr<CPlayer_Walk> CPlayer_Walk::Create()
{
    auto pInstance = unique_ptr<CPlayer_Walk>(new CPlayer_Walk(10.f));

    return pInstance;
}