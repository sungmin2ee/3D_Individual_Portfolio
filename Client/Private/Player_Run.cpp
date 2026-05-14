#include "Player_Run.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Player_Attack.h"

CPlayer_Run::CPlayer_Run(_float velocity) : State<CBody_Player>(velocity)
{
}

CPlayer_Run::~CPlayer_Run()
{
}

void CPlayer_Run::Enter(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        //owner.Get_Model()->Set_Animation(12,false);
        owner.Get_Model()->Set_Animation(17, 0.7f);

    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(21);
    }
}

void CPlayer_Run::Update(CBody_Player& owner, _float deltaTime)
{
    if (CGameInstance::Get().Key_Up(DIK_LSHIFT)) {
         if (CGameInstance::Get().Key_Pressing(DIK_LEFT) || CGameInstance::Get().Key_Pressing(DIK_RIGHT)) {
             owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
         }
         else {
             owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
         }

         
    }
    else if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
        if (!owner.Get_Rotating()) {
            if (CGameInstance::Get().Key_Pressing(DIK_LEFT)
                || CGameInstance::Get().Key_Pressing(DIK_RIGHT)) {

                if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
                    owner.Get_Transform()->Go_Straight(deltaTime * 1.5f);
                }
                else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
                    owner.Get_Transform()->Go_Straight(deltaTime * 2.5f);
                }
            }
        }
    }

    if (CGameInstance::Get().Key_Up(DIK_LEFT) || CGameInstance::Get().Key_Up(DIK_RIGHT)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());

    }
    if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Attack::Create());
    }
}

void CPlayer_Run::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Run> CPlayer_Run::Create()
{
    auto pInstance = unique_ptr<CPlayer_Run>(new CPlayer_Run(10.f));

    return pInstance;
}