#include "Player_OpenDoor.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Door.h"
#include "Blocker.h"

CPlayer_OpenDoor::CPlayer_OpenDoor()
{
}

CPlayer_OpenDoor::~CPlayer_OpenDoor()
{
}

void CPlayer_OpenDoor::Enter(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::DOOR_OPEN_SNEAK), false);
    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::DOOR_OPEN_STAND), false);
    }
    animStart = true;

    auto pCollidedDoor = owner.Get_CollidedDoor();

    auto myPos = owner.Get_Transform()->Get_State(STATE::POSITION);
    auto doorPos = pCollidedDoor->Get_Transform()->Get_State(STATE::POSITION);

    _float4 deltaPos;
    XMStoreFloat4(&deltaPos, (myPos - doorPos));
    pCollidedDoor->Set_DoorOpened(true);

    if (deltaPos.x >= 0) {
        if (pCollidedDoor->Get_LeftBlocker() != nullptr) {
            pCollidedDoor->Get_LeftBlocker()->Set_DoorOpen(true);
            pCollidedDoor->Get_LeftBlocker()->Set_DoorClose(false);
            pCollidedDoor->Get_LeftBlocker()->Set_TransitionFinished(false);
        }

    }
    else {
        if (pCollidedDoor->Get_RightBlocker() != nullptr) {
            pCollidedDoor->Get_RightBlocker()->Set_DoorOpen(true);
            pCollidedDoor->Get_RightBlocker()->Set_DoorClose(false);
            pCollidedDoor->Get_RightBlocker()->Set_TransitionFinished(false);
        }
    }
    owner.Get_CollidedDoor()->Set_Rotating(true);

}

void CPlayer_OpenDoor::Update(CBody_Player& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }

    if (!animStart) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
    }

    //owner.CheckDoorCollide();


}

void CPlayer_OpenDoor::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_OpenDoor> CPlayer_OpenDoor::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_OpenDoor>(new CPlayer_OpenDoor());

    return pInstance;
}