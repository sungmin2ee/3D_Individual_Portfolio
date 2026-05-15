#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Attack :
    public State<CBody_Player>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CPlayer_Attack();

public:
    virtual ~CPlayer_Attack();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Attack> Create();

private:
    _bool animStart = false;
    uint32_t m_iCount = 0;
    _float m_fTime = 0;
};
NS_END
