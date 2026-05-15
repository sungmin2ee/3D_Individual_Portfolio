#include "Player_Execute.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Idle.h"
#include "Player_Walk.h"

CPlayer_Execute::CPlayer_Execute()
{
}

CPlayer_Execute::~CPlayer_Execute()
{
}

void CPlayer_Execute::Enter(CBody_Player& owner)
{
    if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
        owner.Get_Model()->Set_Animation(29, false);
        m_iCount++;
    }
    else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
        owner.Get_Model()->Set_Animation(2, false);
        m_iCount++;
    }
    animStart = true;
}

void CPlayer_Execute::Update(CBody_Player& owner, _float deltaTime)
{
    m_fTime += deltaTime;
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }

    if (m_fTime >= 1.f) {
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
    }

    if (!animStart) {

        if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
            if (m_iCount % 2 == 0) {
                if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
                    owner.Get_Model()->Set_Animation(29, false);
                }
                else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
                    owner.Get_Model()->Set_Animation(2, false);
                }
                m_iCount++;
            }
            else {
                if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
                    owner.Get_Model()->Set_Animation(30, false);
                }
                else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
                    owner.Get_Model()->Set_Animation(3, false);
                }
                m_iCount++;
            }
            m_fTime = 0.f;
            animStart = true;
        }

        if (CGameInstance::Get().Key_Down(DIK_LEFT) || CGameInstance::Get().Key_Down(DIK_RIGHT)) {
            if (CGameInstance::Get().Key_Down(DIK_LSHIFT)) {
                owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
            }
            else {
                owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
            }
        }
    }




}

void CPlayer_Execute::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Execute> CPlayer_Execute::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Execute>(new CPlayer_Execute());

    return pInstance;
}