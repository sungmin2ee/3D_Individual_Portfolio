#include "Zombie_Run.h"
#include "GameInstance.h"
#include "Zombie_Attack.h"
#include "Zombie_Damaged.h"
#include "Zombie_Fatal.h"
#include "Zombie_Stair.h"
#include "Layer.h"
#include "Player.h"
#include "Body_Player.h"
#include "Zombie.h"

CZombie_Run::CZombie_Run()
{
}

CZombie_Run::~CZombie_Run()
{
}

void CZombie_Run::Enter(CBody_Zombie& owner)
{

    owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::RUN), 0.7f);
}

void CZombie_Run::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    //CHASE PLALYER
    if (owner.Get_Damaged()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Damaged::Create());
        return;
    }
    if (owner.Get_PlayerInRange()) {
        owner.Get_StateMachine()->ChangeState(CZombie_Attack::Create());
        return;
    }

    if (owner.Get_HP() <= 21) {
        owner.Get_StateMachine()->ChangeState(CZombie_Fatal::Create());
        return;
    }
    //플레이어가 도망중인데 계단을 탔으면
    //일단 계속가
    //가다가 계단 콜라이더에 닿으면 계단 상태머신 발동 
    if (owner.Get_PlayerDetected()) {
        auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
        auto player = layer->GetObjectFirst();
        auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
        if (owner.Get_CollidedStair() != nullptr) {
            if (playerBody->Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN ||
                playerBody->Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
                owner.Get_StateMachine()->ChangeState(CZombie_Stair::Create());
                return;
            }
        }
       
    }
    if (!owner.Get_DirChanged()) {
        owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 3.f);
    }

}

void CZombie_Run::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Run> CZombie_Run::Create()
{
    auto pInstance = unique_ptr<CZombie_Run>(new CZombie_Run());

    return pInstance;
}