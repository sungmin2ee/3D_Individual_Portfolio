#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Run :
    public State<CBody_Player>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CPlayer_Run(_float velocity);

public:
    virtual ~CPlayer_Run();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Run> Create();



};
NS_END
