#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Execute :
    public State<CBody_Player>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CPlayer_Execute();

public:
    virtual ~CPlayer_Execute();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Execute> Create();

private:
    _bool animStart = false;
    _float soundTime = 0.f;
    _bool   soundPlayed = false;
    uint32_t    count = 0;
};
NS_END
