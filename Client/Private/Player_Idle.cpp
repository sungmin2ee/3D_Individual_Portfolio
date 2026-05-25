#include "Player_Idle.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Walk.h"
#include "Player_Attack.h"
#include "Player_Damaged.h"
#include "Player_Execute.h"
#include "Layer.h"
#include "Zombie.h"
#include "Player_CloseDoor.h"
#include "Player_Door.h"

CPlayer_Idle::CPlayer_Idle()
{
}

CPlayer_Idle::~CPlayer_Idle()
{
}

void CPlayer_Idle::Enter(CBody_Player& owner)
{

    
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND ) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::IDLE));
        owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);

        return;
    }
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::SNEAK_IDLE));
        owner.Set_CurState(CBody_Player::PLAYER_STATE::SIT);
        return;
    }

}

void CPlayer_Idle::Update(CBody_Player& owner, _float deltaTime)
{
    
    int a = owner.Get_Model()->Get_AnimIndex();
    auto b = owner.Get_CurState();
    if (CGameInstance::Get().Key_Pressing(DIK_A) || CGameInstance::Get().Key_Pressing(DIK_D)) {
        // 만약 W키를 누르면 Run 상태로 변경
         owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
    }
    if (CGameInstance::Get().Key_Down(DIK_C)) {

        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::IDLE));
            animStart = false;
            owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
        }
        else {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::SNEAK_IDLE_TRANSITION), false);
            animStart = true;
            owner.Set_CurState(CBody_Player::PLAYER_STATE::SIT);
        }
    }

    if (animStart) {
        if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::SNEAK_IDLE));
            animStart = false;
        }
    }
    if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON) && !owner.Get_Rotating()) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Attack::Create());
        return;

    }
    if (owner.Get_OnHit()) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Damaged::Create());
        return;
    }

    owner.Execute();


    if (owner.Get_CollidedDoor() != nullptr) {
        if (owner.Get_CollidedDoor()->Get_DoorOpened()) {
            if (CGameInstance::Get().Key_Down(DIK_F)) {
                owner.Get_StateMachine()->ChangeState(CPlayer_CloseDoor::Create());

            }
        }
        else {
            owner.Get_StateMachine()->ChangeState(CPlayer_Door::Create());
        }
        return;
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