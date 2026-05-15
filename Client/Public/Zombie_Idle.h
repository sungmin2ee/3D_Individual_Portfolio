#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Idle :
    public State<CBody_Zombie>
{

public:
    enum IDLE_STATE {IDLE, SIT_IDLE};
private:
    CZombie_Idle();

public:
    virtual ~CZombie_Idle();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Idle> Create();

private:
    _bool animStart = false;
    _float m_fTime = 0.f;
    CBody_Zombie::ZOMBIE_STATE curState = CBody_Zombie::ZOMBIE_STATE::END;
};
NS_END
