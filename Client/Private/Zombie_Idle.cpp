#include "Zombie_Idle.h"
#include "GameInstance.h"
#include "Zombie_Run.h"
#include "Zombie_Walk.h"
#include "Zombie_Attack.h"

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
    }else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2));
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE));
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::PATROL_IDLE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::PATROL_IDLE));
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::IDLE) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::IDLE));
    }
    else if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::LAYDOWN) {
        owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::LAYDOWN));
    }
}

void CZombie_Idle::Update(CBody_Zombie& owner, _float deltaTime)
{
    if (owner.Get_PlayerDetected()&& ! animStart) {
        if (owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE) {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::CRAWL_TRANS),false);
            curState = CBody_Zombie::ZOMBIE_STATE::CRAWL_TRANS;
        }
        else if(owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1){
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1), false);
            curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1;

        }
        else if(owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2){
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS2), false);
            curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS2;
        }
        else if(owner.Get_CurState() == CBody_Zombie::ZOMBIE_STATE::LAYDOWN){
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::GET_UP), false);
            curState = CBody_Zombie::ZOMBIE_STATE::GET_UP;
        }
        else {
            owner.Get_Model()->Set_Animation(ETOUI(CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1,false));
            curState = CBody_Zombie::ZOMBIE_STATE::AGGRO_TRANS1;
        }
        animStart = true;
    }
    else {
        m_fTime += deltaTime;

    }

    if (animStart) {
        if (owner.Get_Model()->Play_Animation(deltaTime) == true) {
            if (curState == CBody_Zombie::ZOMBIE_STATE::CRAWL_TRANS) {
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
    }
}

void CZombie_Idle::Exit(CBody_Zombie& owner)
{
}
unique_ptr<CZombie_Idle> CZombie_Idle::Create()
{
    // 기본 속도를 0 혹은 생성자 인자로 넘김
    auto pInstance = unique_ptr<CZombie_Idle>(new CZombie_Idle());

    return pInstance;
}