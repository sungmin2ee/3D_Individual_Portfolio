#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Damaged :
    public State<CBody_Zombie>
{

private:
    CZombie_Damaged();

public:
    virtual ~CZombie_Damaged();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Damaged> Create();

private:
    _bool animStart = false;
    uint32_t m_iCount = 0;
    _float m_fTime = 0.f;
};
NS_END
