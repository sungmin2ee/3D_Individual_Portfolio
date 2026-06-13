#include "Player_Stair.h"
#include "Player_Walk.h"
#include "GameInstance.h"
#include "Player_Idle.h"
#include "Player.h"
#include "Player_Attack.h"

CPlayer_Stair::CPlayer_Stair()
{
}

CPlayer_Stair::~CPlayer_Stair()
{
}

void CPlayer_Stair::Enter(CBody_Player& owner)
{
    int a = ETOUI(owner.Get_CollidedStair()->Get_State());
    if (owner.Get_CollidedStair()->Get_State() == CStair_Collider::STAIR_COLLIDER::STAIR_UP) {
        owner.Set_CurState(CBody_Player::PLAYER_STATE::STAIR_UP);
    }
    else if (owner.Get_CollidedStair()->Get_State() == CStair_Collider::STAIR_COLLIDER::STAIR_DOWN) {
        owner.Set_CurState(CBody_Player::PLAYER_STATE::STAIR_DOWN);
    }
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
        //플레이어가 왼쪽을 바라보고있으면
        m_fReleaseY = owner.Get_StairPos().y + 0.3f;

        m_eEnter = ENTER::UP;
        if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::LEFT) {
            owner.Set_DirChanged();
            owner.Set_Rotating(true);
        }
    }
    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
        m_fReleaseY = owner.Get_StairPos().y - 0.3f;
        m_eEnter = ENTER::DOWN;

        if (owner.Get_CurDir() == CBody_Player::PLAYER_DIR::RIGHT) {
            owner.Set_DirChanged();
            owner.Set_Rotating(true);
        }
    }
}

void CPlayer_Stair::Update(CBody_Player& owner, _float deltaTime)
{

    //나가는 모션
    if (m_bExiting && !m_bAdjustingExit) {
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_TOP_EXIT), false);
            m_bAdjustingExit = true;
            return;

        }
        else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_BOTTOM_EXIT), false);
            m_bAdjustingExit = true;
            return;
        }
    }
    _float4 myPos;
    XMStoreFloat4(&myPos, owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION));


    if (m_bAdjustingExit) {
        owner.Set_StairMove(true);
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
            if (myPos.z > 0) {
                owner.Get_Player().lock()->Get_Transform()->Go_Right(deltaTime * 1.5f);
                return;

            }
            else {
                m_bAdjustingExit = false;
                owner.Set_StairMove(false);
                owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);
                owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
                return;

            }

        }
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
            if (myPos.z > 0) {
                owner.Get_Player().lock()->Get_Transform()->Go_Left(deltaTime * 1.5f);
                return;

            }
            else {
                m_bAdjustingExit = false;
                owner.Set_StairMove(false);

                owner.Set_CurState(CBody_Player::PLAYER_STATE::STAND);

                owner.Get_StateMachine()->ChangeState(CPlayer_Idle::Create());
                return;
            }

        }
    }

    if (!owner.Get_Rotating() && !m_bAdjustingEnter && !m_bEnterAnimFinished) {
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_BOTTOM_ENTER), false);
            animStart = true;
            m_bAdjustingEnter = true;
        }
        else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_TOP_ENTER), false);
            m_bAdjustingEnter = true;
            animStart = true;
        }
    }
    if (m_bAdjustingEnter) {
        owner.Set_StairMove(true);
        if (!owner.Get_Rotating()) {
            if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
                //플레이어가 계단 콜라이더 보다 왼쪽에있으면
                _float4 newPos;
                if (myPos.x < owner.Get_StairPos().x) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Backward(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x > owner.Get_StairPos().x) {
                        owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                //플레이어가 계단 콜라이더 보다 오른쪽에 있으면

                else if (myPos.x > owner.Get_StairPos().x) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x < owner.Get_StairPos().x) {
                        owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                else {
                }
                if (myPos.z < zOffset) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Right(deltaTime * 1.5f);
                }
                else {
                    //owner.Get_Player().lock()->Get_Transform()->Set_State
                    owner.Set_StairMove(false);

                    m_bAdjustingEnter = false;
                    m_bEnterAnimFinished = true;
                    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_UP));
                        animStart = true;
                    }
                    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_DOWN));
                        animStart = true;
                    }
                   

                }
            }
            else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
                //플레이어가 왼쪽에 있으면
                _float4 newPos;
                if (myPos.x < owner.Get_StairPos().x) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x > owner.Get_StairPos().x) {
                        owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                //플레이어가 계단 콜라이더 보다 오른쪽에 있으면

                else if (myPos.x > owner.Get_StairPos().x) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Backward(deltaTime * 1.1f);
                    XMStoreFloat4(&newPos, owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION));
                    if (newPos.x < owner.Get_StairPos().x) {
                        owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetX(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().x));
                    }
                }
                else {
                }

                if (myPos.z < zOffset) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Left(deltaTime * 1.5f);
                }
                else {
                    //owner.Get_Player().lock()->Get_Transform()->Set_State
                    owner.Set_StairMove(false);

                    m_bAdjustingEnter = false;

                    m_bEnterAnimFinished = true;
                    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_UP));
                        animStart = true;
                    }
                    else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
                        owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_DOWN));
                        animStart = true;
                    }
           

                }
            }
        }
        return;
    }
    if (!m_bAdjustingEnter && !m_bAdjustingExit) {
        if (owner.Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_UP)
            || owner.Get_Model()->Get_AnimIndex() == ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_DOWN)) {
            if (CGameInstance::Get().Key_Down(DIK_A) || CGameInstance::Get().Key_Down(DIK_D)) {
                m_bAnimChanged = false;
            }
        }
    }
   
    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
        if (!owner.Get_Rotating() && !m_bAdjustingEnter) {
            if (CGameInstance::Get().Key_Pressing(DIK_D)) {
                if (!m_bAnimChanged) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_RUN_UP));
                    CGameInstance::Get().PlaySoundLoop(L"Run.wav", CHANNELID::SOUND_EFFECT_PLAYER, 1.f);

                    m_bAnimChanged = true;
                }
                if (CGameInstance::Get().GetCurLevelIndex() == 4 || CGameInstance::Get().GetCurLevelIndex() == 3) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.4f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Up(deltaTime * 0.8f);
                }
                else {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Up(deltaTime * 0.8f);
                }
 
                CheckExit(owner, myPos);
                return;
            }
            if (CGameInstance::Get().Key_Pressing(DIK_A)) {
                m_bAnimChanged = false;
                if (!m_bAnimChanged) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_RUN_DOWN));
                    m_bAnimChanged = true;
                    CGameInstance::Get().PlaySoundLoop(L"Run.wav", CHANNELID::SOUND_EFFECT_PLAYER, 1.f);

                }
                owner.Set_CurState(CBody_Player::PLAYER_STATE::STAIR_DOWN);
                if (CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::STAGE1) || CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::SHELTER)) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.4f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Down(deltaTime * 0.8f);
                }
                else {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Down(deltaTime * 0.8f);
                }

                CheckExit(owner, myPos);

                return;
            }
        }

    }

    if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
      
        if (!owner.Get_Rotating() && !m_bAdjustingEnter) {
            if (CGameInstance::Get().Key_Pressing(DIK_D)) {
                m_bAnimChanged = false;
                if (!m_bAnimChanged) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_RUN_UP));
                    CGameInstance::Get().PlaySoundLoop(L"Run.wav", CHANNELID::SOUND_EFFECT_PLAYER, 1.f);
                    m_bAnimChanged = true;

                }
                owner.Set_CurState(CBody_Player::PLAYER_STATE::STAIR_UP);
                if (CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::STAGE1) || CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::SHELTER)) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.4f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Up(deltaTime * 0.8f);
                }
                else {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Up(deltaTime * 0.8f);
                }

                CheckExit(owner, myPos);

                return;
            }
            if (CGameInstance::Get().Key_Pressing(DIK_A)) {
                if (!m_bAnimChanged) {
                    owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_RUN_DOWN));
                    CGameInstance::Get().PlaySoundLoop(L"Run.wav", CHANNELID::SOUND_EFFECT_PLAYER, 1.f);
                    m_bAnimChanged = true;

                }
                if (CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::STAGE1) || CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::SHELTER)) {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.4f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Down(deltaTime * 0.8f);
                }
                else {
                    owner.Get_Player().lock()->Get_Transform()->Go_Straight(deltaTime * 1.1f);
                    owner.Get_Player().lock()->Get_Transform()->Go_Down(deltaTime * 0.8f);
                }
          
                CheckExit(owner, myPos);

                return;
            }
        }
       
    }
   
    if (CGameInstance::Get().Key_Up(DIK_D)) {
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_DOWN));
            CGameInstance::Get().StopSound(CHANNELID::SOUND_EFFECT_PLAYER);

            return;
        }
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_UP));
            CGameInstance::Get().StopSound(CHANNELID::SOUND_EFFECT_PLAYER);

            return;

        }
    }
    if (CGameInstance::Get().Key_Up(DIK_A)) {
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_DOWN));
            CGameInstance::Get().StopSound(CHANNELID::SOUND_EFFECT_PLAYER);

            return;

        }
        if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Player::PLAYER_ANIM::STAIR_IDLE_UP));
            CGameInstance::Get().StopSound(CHANNELID::SOUND_EFFECT_PLAYER);

            return;

        }
    }


   


}

void CPlayer_Stair::Exit(CBody_Player& owner)
{
    //if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_UP) {
    //    owner.Get_Model()->Set_Animation(14, false);
    //}
    //else if (owner.Get_CurState() == CBody_Player::PLAYER_STATE::STAIR_DOWN) {
    //    owner.Get_Model()->Set_Animation(16, false);
    //}
    CGameInstance::Get().StopSound(CHANNELID::SOUND_EFFECT_PLAYER);
}
void CPlayer_Stair::CheckExit(CBody_Player& owner, _float4 myPos)
{
    _float a = m_fReleaseY;
    if (!m_bExiting) {
        if (m_eEnter == ENTER::UP) {
            if (myPos.y < owner.Get_StairPos().y) {
                m_bExiting = true;
                owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetY(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().y));
            }
            if (myPos.y > m_fReleaseY) {
                m_bExiting = true;
                owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetY(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), m_fReleaseY));

            }
        }
        else if (m_eEnter == ENTER::DOWN) {
            if (myPos.y > owner.Get_StairPos().y) {
                owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetY(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), owner.Get_StairPos().y));
                m_bExiting = true;
            }
            if (myPos.y < m_fReleaseY) {
                m_bExiting = true;
                owner.Get_Player().lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetY(owner.Get_Player().lock()->Get_Transform()->Get_State(STATE::POSITION), m_fReleaseY));
            }
        }
    }
    
}
unique_ptr<CPlayer_Stair> CPlayer_Stair::Create()
{
    auto pInstance = unique_ptr<CPlayer_Stair>(new CPlayer_Stair());

    return pInstance;
}