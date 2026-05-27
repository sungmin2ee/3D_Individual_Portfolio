#include "Player_Attack.h"
#include "Player_Damaged.h"
#include "Player_Run.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Layer.h"
#include "Zombie.h"
#include "Player_Walk.h"

CPlayer_Damaged::CPlayer_Damaged()
{
}

CPlayer_Damaged::~CPlayer_Damaged()
{
}



void CPlayer_Damaged::Enter(CBody_Player& owner)
{
    auto zombies = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Zombie"))->GetObjects();
    auto iter = zombies.begin();

    for (iter; iter != zombies.end(); iter++) {
       // if (owner.Get_Obb()->myOBB.Intersects(static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_Obb()->myOBB)) {
            auto pos = owner.Get_Transform()->Get_State(STATE::POSITION);
            _vector delta = XMVectorSet(0.02f, 0, 0, 0);
            if (ETOUI(static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_CurDir()) == ETOUI(owner.Get_CurDir())) {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::HIT_REACT_FRONT),0.7f, false);
                if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT) {
                    pos = pos - delta;
                }
                else {
                    pos = pos + delta;
                }
                owner.Get_Transform()->Set_State(STATE::POSITION, pos);
                break;
            }
            else {
            
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::HIT_REACT_BACK), 0.7f, false);
                if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT) {
                    pos = pos + delta;
                }
                else {
                    pos = pos - delta;
                }
                owner.Get_Transform()->Set_State(STATE::POSITION, pos);
                break;
            }
      //  }
    }
    m_iCount++;
    animStart = true;
    owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
    owner.Set_Hp(-15);
}

void CPlayer_Damaged::Update(CBody_Player& owner, _float deltaTime)
{
 
    m_fTime += deltaTime;
    if (!animStart) {
        if (CGameInstance::Get().Key_Pressing(DIK_A) || CGameInstance::Get().Key_Pressing(DIK_D)) {
            // 만약 W키를 누르면 Run 상태로 변경
            if (CGameInstance::Get().Key_Pressing(DIK_LSHIFT)) {
                owner.Get_StateMachine()->ChangeState(CPlayer_Run::Create());
                return;
            }
            owner.Get_StateMachine()->ChangeState(CPlayer_Walk::Create());
            return;
        }
    }

  
    if (m_fTime >= 0.5f) {
        owner.Set_OnHit(false);
        if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
            owner.Get_StateMachine()->ChangeState(CPlayer_Attack::Create());
            return;
        }
       
    }
    if (m_fTime >= 1.f) {
        owner.Set_OnHit(false);
        owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
        return;
    }

    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
}

void CPlayer_Damaged::Exit(CBody_Player& owner)
{
    owner.Set_OnHit(false);
}
unique_ptr<CPlayer_Damaged> CPlayer_Damaged::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CPlayer_Damaged>(new CPlayer_Damaged());

    return pInstance;
}