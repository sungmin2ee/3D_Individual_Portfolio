#include "Player_CloseDoor.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Door.h"
#include "Blocker.h"
#include "Player.h"

CPlayer_CloseDoor::CPlayer_CloseDoor()
{
}

CPlayer_CloseDoor::~CPlayer_CloseDoor()
{
}

void CPlayer_CloseDoor::Enter(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::DOOR_PULL_CLOSE_SNEAK),false);
    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::DOOR_PULL_CLOSE_STAND),false);
    }
    animStart = true;

    auto pCollidedDoor = owner.Get_CollidedDoor();
    auto myPos = owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION);

    //auto myPos = owner.Get_Transform()->Get_State(STATE::POSITION);
    auto doorPos = pCollidedDoor->Get_Transform()->Get_State(STATE::POSITION);

    _float4 deltaPos;
    XMStoreFloat4(&deltaPos, (myPos - doorPos));

    pCollidedDoor->Set_DoorOpened(false);
    if (deltaPos.x >= 0) {
        if (pCollidedDoor->Get_LeftBlocker() != nullptr) {
            pCollidedDoor->Get_LeftBlocker()->Set_DoorClose(true);
            pCollidedDoor->Get_LeftBlocker()->Set_DoorOpen(false);
            pCollidedDoor->Get_LeftBlocker()->Set_TransitionFinished(false);
        }

    }
    else {
        if (pCollidedDoor->Get_RightBlocker() != nullptr) {
            pCollidedDoor->Get_RightBlocker()->Set_DoorClose(true);
            pCollidedDoor->Get_RightBlocker()->Set_DoorOpen(false);
            pCollidedDoor->Get_RightBlocker()->Set_TransitionFinished(false);
        }

    }
    owner.Get_CollidedDoor()->Set_Rotating(true);

}

void CPlayer_CloseDoor::Update(CBody_Player& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;

    }
        
    if (!animStart) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        owner.Get_Transform()->Go_Backward(deltaTime * 4.f);
        return;
    }


}

void CPlayer_CloseDoor::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_CloseDoor> CPlayer_CloseDoor::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_CloseDoor>(new CPlayer_CloseDoor());

    return pInstance;
}