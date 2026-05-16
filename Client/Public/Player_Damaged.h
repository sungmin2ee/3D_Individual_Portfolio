#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CPlayer_Damaged :
    public State<CBody_Player>
{

private:
    CPlayer_Damaged();

public:
    virtual ~CPlayer_Damaged();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Damaged> Create();

private:
    _bool animStart = false;
    uint32_t m_iCount = 0;
    _float m_fTime = 0.f;
};
NS_END
