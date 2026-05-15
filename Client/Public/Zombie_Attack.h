#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Attack :
    public State<CBody_Zombie>
{

private:
    CZombie_Attack();

public:
    virtual ~CZombie_Attack();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Attack> Create();

private:
    _bool animStart = false;
    uint32_t m_iCount = 0;
};
NS_END
