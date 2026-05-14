#include "Player_Attack.h"
#include "GameInstance.h"
#include "Player_Run.h"
#include "Player_Walk.h"

CPlayer_Attack::CPlayer_Attack(_float velocity) : State<CBody_Player>(velocity)
{
}

CPlayer_Attack::~CPlayer_Attack()
{
}

void CPlayer_Attack::Enter(CBody_Player& owner)
{
    if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::HAND) {
        owner.Get_Model()->Set_Animation(29,false);
        m_iCount++;
    }
    else if (owner.Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
        owner.Get_Model()->Set_Animation(2, false);
        m_iCount++;
    }
    animStart = true;
}

void CPlayer_Attack::Update(CBody_Player& owner, _float deltaTime)
{
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
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

void CPlayer_Attack::Exit(CBody_Player& owner)
{
}
unique_ptr<CPlayer_Attack> CPlayer_Attack::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Attack>(new CPlayer_Attack(0.f));

    return pInstance;
}