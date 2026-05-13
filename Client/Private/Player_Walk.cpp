#include "Player_Run.h"
#include "Player_Walk.h"
#include "GameInstance.h"
#include "Player_Idle.h"

CPlayer_Walk::CPlayer_Walk(_float velocity) : State<CBody_Player>(velocity)
{
}

CPlayer_Walk::~CPlayer_Walk()
{
}

void CPlayer_Walk::Enter(CBody_Player& owner)
{
    owner.Get_Model()->Set_Animation(22,false);
}

void CPlayer_Walk::Update(CBody_Player& owner, _float deltaTime)
{

    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        owner.Get_Model()->Set_Animation(21);
    }
    if (CGameInstance::Get().Key_Pressing(DIK_LEFT) || CGameInstance::Get().Key_Pressing(DIK_RIGHT)) {
        
    }
    //if (CGameInstance::Get().Key_Up(DIK_LEFT) || CGameInstance::Get().Key_Up(DIK_RIGHT)) {
    //    owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
    //}
    if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
    }
}

void CPlayer_Walk::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Walk> CPlayer_Walk::Create()
{
    auto pInstance = unique_ptr<CPlayer_Walk>(new CPlayer_Walk(10.f));

    return pInstance;
}