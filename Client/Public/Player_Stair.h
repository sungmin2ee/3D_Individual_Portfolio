#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Stair :
    public State<CBody_Player>
{

public:
    enum STAIR_STATE { DOWN, DOWNFAST,UP, UPFAST, IDLE_DOWN, IDLE_UP,END };
private:
    CPlayer_Stair(_float velocity);

public:
    virtual ~CPlayer_Stair();
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;

public:
    static unique_ptr<CPlayer_Stair> Create();


    _bool animStart = false;
    STAIR_STATE m_eCurstate = IDLE_UP;
    STAIR_STATE m_ePrestate = END;
};
NS_END
