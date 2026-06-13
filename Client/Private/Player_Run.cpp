#include "Player_Run.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Player_Attack.h"
#include "Player_Damaged.h"
#include "Player_Door.h"
#include "Player_CloseDoor.h"
#include "Player_Stair.h"
#include "Player.h"
#include "Layer.h"

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
        CGameInstance::Get().PlaySoundLoop(L"Run.wav", CHANNELID::SOUND_EFFECT_PLAYER, 1.f);
    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::SIT) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::SNEAK_WALK));
        owner.Set_MakingSound(false);

    }
}

void CPlayer_Run::Update(CBody_Player& owner, _float deltaTime)
{
    runTime += deltaTime;
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
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.5f);
                }
                else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAND) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 2.5f);

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

void CPlayer_Run::Exit(CBody_Player& owner)
{
   
    owner.Set_MakingSound(false);
    CGameInstance::Get().StopSound(CHANNELID::SOUND_EFFECT_PLAYER);
    if (runTime > 2.f) {
        CGameInstance::Get().PlaySoundOne(L"male_outofbreath_06.wav", CHANNELID::SOUND_EFFECT_PLAYER, 1.f);
    }
    //CGameInstance::Get().StopAll();


}
unique_ptr<CPlayer_Run> CPlayer_Run::Create()
{
    auto pInstance = unique_ptr<CPlayer_Run>(new CPlayer_Run());

    return pInstance;
}