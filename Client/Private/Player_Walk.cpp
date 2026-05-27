#include "Player_Run.h"
#include "Player_Walk.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Player_Attack.h"
#include "Player_Damaged.h"
#include "Player_CloseDoor.h"
#include "Player_Door.h"
#include "Player_Stair.h"
#include "Layer.h"
#include "Door.h"

CPlayer_Walk::CPlayer_Walk()
{
}

CPlayer_Walk::~CPlayer_Walk()
{
}

void CPlayer_Walk::Enter(CBody_Player& owner)
{
    CBody_Player::PLAYER_STATE a = owner.Get_CurState();
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::SNEAK_WALK_START), true);
        owner.Set_MakingSound(false);

    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::WALK), true);
        owner.Set_MakingSound(true);

    }
}

void CPlayer_Walk::Update(CBody_Player& owner, _float deltaTime)
{


    if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT && !owner.Get_Rotating()) {
        if (CGameInstance::Get().Key_Pressing(DIK_A)) {
            if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
                owner.Get_Transform()->Go_Straight(deltaTime * 0.7f);
            }
            else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
                owner.Get_Transform()->Go_Straight(deltaTime);
            }
        }
        
    }
    if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::RIGHT && !owner.Get_Rotating()) {
        if (CGameInstance::Get().Key_Pressing(DIK_D)) {
            if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
                owner.Get_Transform()->Go_Straight(deltaTime * 0.7f);
            }
            else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
                owner.Get_Transform()->Go_Straight(deltaTime);
            }
        }
    
    }

    if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT) && 
        (CGameInstance::Get().Key_Pressing(DIK_A)|| CGameInstance::Get().Key_Pressing(DIK_D))) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
        return;

    }

    if (CGameInstance::Get().Key_Up(DIK_A) || CGameInstance::Get().Key_Up(DIK_D)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
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
    if (owner.Get_CollidedStair() != nullptr) {
        if (CGameInstance::Get().Key_Down(DIK_F)) {
            owner.Get_StateMachine()->ChangeState(CPlayer_Stair::Create());
            return;
        }
    }
}

void CPlayer_Walk::Exit(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::WALKSTOP), false);
    }
    owner.Set_MakingSound(false);

}
unique_ptr<CPlayer_Walk> CPlayer_Walk::Create()
{
    auto pInstance = unique_ptr<CPlayer_Walk>(new CPlayer_Walk());

    return pInstance;
}