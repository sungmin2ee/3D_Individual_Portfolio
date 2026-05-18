#include "Player_Run.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Player_Attack.h"
#include "Player_Damaged.h"

CPlayer_Run::CPlayer_Run()
{
}

CPlayer_Run::~CPlayer_Run()
{
}

void CPlayer_Run::Enter(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        //owner.Get_Model()->Set_Animation(12,false);
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::RUN_MED), 0.7f);
        owner.Set_MakingSound(true);

    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::SNEAK_WALK));
        owner.Set_MakingSound(false);

    }
}

void CPlayer_Run::Update(CBody_Player& owner, _float deltaTime)
{
    if (owner.Get_OnHit()) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Damaged::Create());
        return;
    }
    if (CGameInstance::Get().Key_Up(DIK_LSHIFT)) {
         if (CGameInstance::Get().Key_Pressing(DIK_A) || CGameInstance::Get().Key_Pressing(DIK_D)) {
             owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
             return;
         }
         else {
             owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
             return;
         }
    }
    else if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
        if (!owner.Get_Rotating()) {
            if (CGameInstance::Get().Key_Pressing(DIK_A)
                || CGameInstance::Get().Key_Pressing(DIK_D)) {

                if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
                    owner.Get_Transform()->Go_Straight(deltaTime * 1.5f);
                }
                else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
                    owner.Get_Transform()->Go_Straight(deltaTime * 2.5f);
                }
            }
        }
    }

    if (CGameInstance::Get().Key_Up(DIK_A) || CGameInstance::Get().Key_Up(DIK_D)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;


    }
    if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON) && !owner.Get_Rotating()) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Attack::Create());
        return;

    }
    owner.Execute();
}

void CPlayer_Run::Exit(CBody_Player& owner)
{
    owner.Set_MakingSound(false);

}
unique_ptr<CPlayer_Run> CPlayer_Run::Create()
{
    auto pInstance = unique_ptr<CPlayer_Run>(new CPlayer_Run());

    return pInstance;
}