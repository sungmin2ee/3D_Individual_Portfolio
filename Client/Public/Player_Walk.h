#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Walk :
    public State<CBody_Player>
{

public:
    enum IDLE_STATE { IDLE, SIT_IDLE };
private:
    CPlayer_Walk();

public:
    virtual ~CPlayer_Walk();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Walk> Create();


    _bool animStart = false;

};
NS_END
