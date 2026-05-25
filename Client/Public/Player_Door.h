#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Door :
    public State<CBody_Player>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CPlayer_Door();

public:
    virtual ~CPlayer_Door();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Door> Create();

private:
    _bool animStart = false;
};
NS_END
