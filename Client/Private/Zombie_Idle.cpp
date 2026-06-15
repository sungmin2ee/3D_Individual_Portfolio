#include "Zombie_Idle.h"
#include "GameInstance.h"
#include "Zombie_Run.h"
#include "Zombie_Walk.h"
#include "Zombie_Attack.h"
#include "Zombie_StealthDeath.h"

CZombie_Idle::CZombie_Idle()
{
}

CZombie_Idle::~CZombie_Idle()
{
}

void CZombie_Idle::Enter(CBody_Zombie& owner)
{
    if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1));
        CGameInstance::Get().PlaySoundLoop(L"zombieIdle2.wav", owner.Get_RunChannelPtr(), 0.0f);


    }else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2));
        CGameInstance::Get().PlaySoundLoop(L"zombieIdle2.wav", owner.Get_RunChannelPtr(), 0.0f);

    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE));
        CGameInstance::Get().PlaySoundLoop(L"zombieIdle2.wav", owner.Get_RunChannelPtr(), 0.0f);

    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::PATROL_IDLE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::PATROL_IDLE));
        CGameInstance::Get().PlaySoundLoop(L"zombieIdle2.wav", owner.Get_RunChannelPtr(), 0.0f);

    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::IDLE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::IDLE));
        CGameInstance::Get().PlaySoundLoop(L"zombieIdle2.wav", owner.Get_RunChannelPtr(), 0.0f);

    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::LAYDOWN) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::LAYDOWN));
        //CGameInstance::Get().PlaySoundLoop(L"zombieIdle2.wav", owner.Get_RunChannelPtr(), 0.0f);

    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::DOORHIT) {
        
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::DOORHIT), CGameInstance::Get().Random(0.5f, 1.f));
        CGameInstance::Get().PlaySoundLoop(L"doorHit.wav", owner.Get_RunChannelPtr(), 0.0f);

    }
   

}

void CZombie_Idle::Update(CBody_Zombie& owner, _float deltaTime)
{
    //좀비가 방향을 바꾸는 도중이 아니면
    if (owner.Get_RunChannel())
    {
        CGameInstance::Get().SetChannelVolume(owner.Get_RunChannelPtr(), owner.Get_Volume());
    }
    //auto flag = owner.Get_PlayerDetected();
    if (!owner.Get_Rotating() && !owner.Get_DirChanged()) {
        //좀비가 플레이어를 발견했으면
        if (owner.Get_PlayerDetected() &&!flag) {
            //발견 애니메이션 발동
            if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE) {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::CRAWL_TRANS), 0.5f, false);
                curState = CBody_Zombie::ZOMBIE_STATE::CRAWL_TRANS;
            }
            else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1) {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1), 0.6f, false);
                curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1;

            }
            else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2) {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS2), 0.6f, false);
                curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS2;
            }
            else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::LAYDOWN) {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::GET_UP), false);
                curState = CBody_Zombie::ZOMBIE_STATE::GET_UP;
            }
            else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::PATROL_IDLE) {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1), 0.6f, false);
                curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1;
            }
            else {
                owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1, 0.6f, false));
                curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1;
            }
            animStart = true;
            flag = true;

        }
   
    }

 /*   if (owner.Get_PlayerDetected() && animStart) {
        m_fTime += deltaTime;
    }*/
    // 발견 애니메이션이 끝났으면
  
    if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
        animStart = false;
    }
    if (!animStart && owner.Get_PlayerDetected()) {
        if (curState == CBody_Zombie::ZOMBIE_STATE::CRAWL_TRANS || curState == CBody_Zombie::ZOMBIE_STATE::GET_UP) {
            owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
            owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::RUN);
            return;
        }
        else {
            srand((unsigned int)time(0));
            uint32_t random = rand() % 2;
            if (random == 0) {
                owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::RUN);
                owner.Get_StateMachine()->ChangeState(CZombie_Run::Create());
                return;
            }
            else {
                uint32_t iRandom = rand() % 3;
                if (iRandom == 0) {
                    owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::WALK_FAST);
                }
                else if (iRandom == 1) {
                    owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::PATROL_WALK);
                }
                else if (iRandom == 2) {
                    owner.Set_CurState(CBody_Zombie::ZOMBIE_STATE::SEARCH_WALK);
                }
                owner.Get_StateMachine()->ChangeState(CZombie_Walk::Create());
                return;
            }
        }
    }
    if (owner.Stealth_Death()) {
        owner.Get_StateMachine()->ChangeState(CZombie_StealthDeath::Create());
        return;
    }
}

void CZombie_Idle::Exit(CBody_Zombie& owner)
{
    if (owner.Get_RunChannel())
    {
        CGameInstance::Get().StopSound(owner.Get_RunChannelPtr());
    }
}
unique_ptr<CZombie_Idle> CZombie_Idle::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CZombie_Idle>(new CZombie_Idle());

    return pInstance;
}