#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Fatal :
    public State<CBody_Zombie>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CZombie_Fatal();

public:
    virtual ~CZombie_Fatal();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Fatal> Create();

private:
    _bool animStart = false;
    _bool m_bDead = false;
    _bool animFinished = false;
    _float animTimer = 0;
};
NS_END
