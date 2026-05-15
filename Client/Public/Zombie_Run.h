#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Run :
    public State<CBody_Zombie>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CZombie_Run();

public:
    virtual ~CZombie_Run();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Run> Create();

private:
    _bool animStart = false;
    uint32_t random = 100;
};
NS_END
