#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Walk :
    public State<CBody_Zombie>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CZombie_Walk();

public:
    virtual ~CZombie_Walk();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Walk> Create();
    _bool animStart = false;
    _bool detectedAnimStart = false;
};
NS_END
