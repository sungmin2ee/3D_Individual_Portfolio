#include "Player_Run.h"
#include "GameInstance.h"
#include "Player_Idle.h"

CPlayer_Run::CPlayer_Run(_float velocity) : State<CBody_Player>(velocity)
{
}

CPlayer_Run::~CPlayer_Run()
{
}

void CPlayer_Run::Enter(CBody_Player& owner)
{
    owner.Get_Model()->Set_Animation(4);
}

void CPlayer_Run::Update(CBody_Player& owner, _float deltaTime)
{
    if (CGameInstance::Get().Key_Down(DIK_LEFT)) {
         owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
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