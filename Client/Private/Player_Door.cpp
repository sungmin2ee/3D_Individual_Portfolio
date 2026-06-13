#include "Player_Door.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Player_OpenDoor.h"
#include "Door.h"
#include "Blocker.h"
#include "Player.h"

CPlayer_Door::CPlayer_Door()
{
}

CPlayer_Door::~CPlayer_Door()
{
}

void CPlayer_Door::Enter(CBody_Player& owner)
{
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::DOOR_PEEK_SNEAK_IDLE),false);
    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::IDLE),false);
    }
    animStart = true;
}

void CPlayer_Door::Update(CBody_Player& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }

    if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT) {
        if (CGameInstance::Get().Key_Pressing(DIK_D)) {
            if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
                owner.Get_Player().lock()->Get_Transform()->Go_Backward(deltaTime * 4.f);
                owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
                return;
            }
            owner.Get_Player().lock()->Get_Transform()->Go_Backward(deltaTime * 4.f);
            //owner.Set_CurDir(CBody_Player::PLAYER_DIR::RIGHT);
            owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
            return;
        }
    }
    if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::RIGHT) {
        if (CGameInstance::Get().Key_Pressing(DIK_A)) {
            //owner.Set_CurDir(CBody_Player::PLAYER_DIR::LEFT);
            if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
                owner.Get_Player().lock()->Get_Transform()->Go_Backward(deltaTime * 4.f);
                owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
                return;
            }
            owner.Get_Player().lock()->Get_Transform()->Go_Backward(deltaTime * 4.f);
            owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
            return;
        }
    }
    if (!animStart) {
      
    }
    if (CGameInstance::Get().Key_Down(DIK_F)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_OpenDoor::Create());
        CGameInstance::Get().PlaySoundLoop(L"dooropen.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, 1.f);
        return;
    }







}

void CPlayer_Door::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Door> CPlayer_Door::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Door>(new CPlayer_Door());

    return pInstance;
}