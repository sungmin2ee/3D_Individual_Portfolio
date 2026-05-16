#include "Player_Attack.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Idle.h"
#include "Player_Walk.h"
#include "Player_Damaged.h"
#include "Layer.h"
#include "Zombie.h"

CPlayer_Attack::CPlayer_Attack()
{
}

CPlayer_Attack::~CPlayer_Attack()
{
}

void CPlayer_Attack::Enter(CBody_Player& owner)
{

    owner.Set_CurState(CBody_Player::PLAYER_STATE::ATTACK);
    if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::UNARMED_ATTACK1),1.5f,false);
        m_iCount++;
    }
    else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::AXE_ATTACK1), false);
        m_iCount++;
    }
   auto zombies = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Zombie"))->GetObjects();
   auto iter = zombies.begin();
   for (iter; iter != zombies.end(); iter++) {
       if (owner.Get_Obb()->myOBB.Intersects(static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_Obb()->myOBB)) {
           static_pointer_cast<CZombie>(*iter)->Get_Body()->Set_Damaged();
       }
   }
    animStart = true;
}

void CPlayer_Attack::Update(CBody_Player& owner, _float deltaTime)
{

    m_fTime += deltaTime;
    auto zombies = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Zombie"))->GetObjects();
    auto iter = zombies.begin();


    if (owner.Get_OnHit()) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Damaged::Create());
        return;
    }
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }

    if (m_fTime >= 1.f) {
        owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
    }

    if (!animStart) {

        if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
            if (m_iCount % 2 == 0) {
                if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::UNARMED_ATTACK1), 1.5f, false);
                }
                else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::AXE_ATTACK2), false);
                }
                m_iCount++;
            }
            else {
                if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::UNARMED_ATTACK2), 1.5f, false);
                }
                else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::AXE_ATTACK2), false);
                }
                m_iCount++;
            }
            m_fTime = 0.f;
            animStart = true;
            for (iter; iter != zombies.end(); iter++) {
                if (owner.Get_Obb()->myOBB.Intersects(static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_Obb()->myOBB)) {
                    static_pointer_cast<CZombie>(*iter)->Get_Body()->Set_Damaged();
                }
            }
            return;
        }
   
        if (CGameInstance::Get().Key_Pressing(DIK_A) || CGameInstance::Get().Key_Pressing(DIK_D)) {
            if (CGameInstance::Get().Key_Down(DIK_LSHIFT)) {
                owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
                owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
                return;
            }
            owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
            owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
            return;
        }
    }

   


}

void CPlayer_Attack::Exit(CBody_Player& owner)
{
    auto a = owner.Get_CurState();

}
unique_ptr<CPlayer_Attack> CPlayer_Attack::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Attack>(new CPlayer_Attack());

    return pInstance;
}