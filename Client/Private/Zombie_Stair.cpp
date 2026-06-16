#include "Zombie_Stair.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Player.h"
#include "Body_Player.h"
#include "Stair_Collider.h"
#include "Zombie_Run.h"
#include "Zombie.h"


CZombie_Stair::CZombie_Stair()
{
}

CZombie_Stair::~CZombie_Stair()
{
}

void CZombie_Stair::Enter(CBody_Zombie& owner)
{
    auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
    auto player = layer->GetObjectFirst();
    auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
    if (playerBody->Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER);
        m_fReleaseY = owner.Get_StairPos().y + 0.3f;
        if (owner.Get_CurDir() == CBody_Zombie::ZOMBIE_DIR::LEFT) {
            owner.Set_DirChanged();
        }
    }else if (playerBody->Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER);
        m_fReleaseY = owner.Get_StairPos().y - 0.3f;
        if (owner.Get_CurDir() == CBody_Zombie::ZOMBIE_DIR::RIGHT) {
            owner.Set_DirChanged();
        }   
    }

    int a = ETOUI(owner.Get_CurState());


    owner.Set_UsingStair(true);
    CGameInstance::Get().PlaySoundLoop(L"zombieRun.wav", owner.Get_RunChannelPtr(), 1.0f);

}

void CZombie_Stair::Update(CBody_Zombie& owner, _float deltaTime)
{
    owner.Get_Model()->Play_Animation(deltaTime);
    
    _float4 myPos;
    XMStoreFloat4(&myPos, owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION));

    if (!owner.Get_Rotating() && !m_bAdjustingEnter && !m_bEnterAnimFinished) {
        if (owner.Get_CollidedStair()->Get_State() == CStair_Collider::STAIR_COLLIDER::STAIR_UP) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER), false);
            animStart = true;
            m_bAdjustingEnter = true;

        }
        else  if (owner.Get_CollidedStair()->Get_State() == CStair_Collider::STAIR_COLLIDER::STAIR_DOWN) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER), false);
            m_bAdjustingEnter = true;
            animStart = true;
        }
    }
    if (m_bAdjustingEnter) {
        if (!owner.Get_Rotating()) {
            if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER) {
                //플레이어가 계단 콜라이더 보다 왼쪽에있으면
                _float4 newPos;
                if (myPos.x < owner.Get_StairPos().x) {
                    owner.Get_Zombie().lock()->Get_Transform()->Go_Backward(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x > owner.Get_StairPos().x) {
                        owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                //플레이어가 계단 콜라이더 보다 오른쪽에 있으면

                else if (myPos.x > owner.Get_StairPos().x) {
                    owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x < owner.Get_StairPos().x) {
                        owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                else {
                }
                if (myPos.z < zOffset) {
                    owner.Get_Zombie().lock()->Get_Transform()->Go_Right(deltaTime * 1.5f);
                }
                else {
                    //owner.Get_Transform()->Set_State
                    m_bAdjustingEnter = false;
                    m_bEnterAnimFinished = true;
                    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP));
                        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP);

                        animStart = true;
                    }
                    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN));
                        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN);

                        animStart = true;
                    }
                }
            }
            else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER) {
                //플레이어가 왼쪽에 있으면
                _float4 newPos;
                if (myPos.x < owner.Get_StairPos().x) {
                    owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x > owner.Get_StairPos().x) {
                        owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                //플레이어가 계단 콜라이더 보다 오른쪽에 있으면

                else if (myPos.x > owner.Get_StairPos().x) {
                    owner.Get_Zombie().lock()->Get_Transform()->Go_Backward(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x < owner.Get_StairPos().x) {
                        owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                else {
                }

                if (myPos.z < zOffset) {
                    owner.Get_Zombie().lock()->Get_Transform()->Go_Left(deltaTime * 1.5f);
                }
                else {

                    m_bAdjustingEnter = false;

                    m_bEnterAnimFinished = true;
                    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_ENTER) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP));
                        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP);
                        animStart = true;
                    }
                    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_ENTER) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN));
                        owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN);

                        animStart = true;
                    }
                }
            }
        }
       
        return;
    }
    if (!m_bExiting && !m_bAdjustingExit && !m_bAdjustingEnter) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP) {
            if (CGameInstance::Get().GetCurLevelIndex() == 4) {
                owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 0.9f);
                owner.Get_Zombie().lock()->Get_Transform()->Go_Up(deltaTime * 0.5f);
            }
            else {
                owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 0.9f);
                owner.Get_Zombie().lock()->Get_Transform()->Go_Up(deltaTime * 0.75f);
            }
   
            CheckExit(owner, myPos);

        }
        else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN) {
            if (CGameInstance::Get().GetCurLevelIndex() == 4) {
                owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 1.3f);
                owner.Get_Zombie().lock()->Get_Transform()->Go_Down(deltaTime * 0.8f);

            }
            else {
                owner.Get_Zombie().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                owner.Get_Zombie().lock()->Get_Transform()->Go_Down(deltaTime * 0.8f);
            }

            CheckExit(owner, myPos);
        }
    }

    if (m_bExiting && !m_bAdjustingExit) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_EXIT), false);
            owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_EXIT);

            m_bAdjustingExit = true;
            return;

        }
        else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_EXIT), false);
            owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_EXIT);

            m_bAdjustingExit = true;
            return;
        }
    }

    if (m_bAdjustingExit) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_TOP_EXIT) {
            if (myPos.z > 0) {
                owner.Get_Zombie().lock()->Get_Transform()->Go_Right(deltaTime * 1.5f);
                return;

            }
            else {
                m_bAdjustingExit = false;
                owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::RUN);
                owner.Set_UsingStair(false);

                owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
                return;

            }

        }
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_BOTTOM_EXIT) {
            if (myPos.z > 0) {
                owner.Get_Zombie().lock()->Get_Transform()->Go_Left(deltaTime * 1.5f);
                return;

            }
            else {
                m_bAdjustingExit = false;
                owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::RUN);
                owner.Set_UsingStair(false);
                owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
                return;
            }

        }
    }
}

void CZombie_Stair::Exit(CBody_Zombie& owner)
{
    if (owner.Get_RunChannel())
    {
        CGameInstance::Get().StopSound(owner.Get_RunChannelPtr());
    }
}
unique_ptr<CZombie_Stair> CZombie_Stair::Create()
{
    auto pInstance = unique_ptr<CZombie_Stair>(new CZombie_Stair());

    return pInstance;
}
void CZombie_Stair::CheckExit(CBody_Zombie& owner, _float4 myPos)
{
    _float a = m_fReleaseY;
    if (!m_bExiting) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_UP) {
            if (myPos.y > m_fReleaseY) {
                m_bExiting = true;
                owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetY(owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION), m_fReleaseY));
            }
        }
        else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::STAIR_WALK_DOWN) {
            if (myPos.y < m_fReleaseY) {
                m_bExiting = true;
                owner.Get_Zombie().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetY(owner.Get_Zombie().lock()->Get_Transform()->Get_State(STATE::POSITION), m_fReleaseY));
            }
        }
    }

}