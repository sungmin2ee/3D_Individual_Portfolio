#pragma once

#include "State.h"
#include "Body_Player.h"
NS_BEGIN(Client)
class CPlayer_Stair :
    public State<CBody_Player>
{


private:
    CPlayer_Stair();

public:
    virtual ~CPlayer_Stair();
public:
    enum ENTER { UP, DOWN, END};
public:
    virtual void Enter(CBody_Player& owner) override;
    virtual void Update(CBody_Player& owner, _float deltaTime) override;
    virtual void Exit(CBody_Player& owner) override;
    void         CheckExit(CBody_Player& owner, _float4 myPos);
public:
    static unique_ptr<CPlayer_Stair> Create();


    _bool animStart = false;
    _bool m_bAdjustingEnter = false;
    _bool m_bAdjustingExit = false;
    _bool m_bAnimChanged = false;
    _bool m_bExiting = false;
    _bool m_bEnterAnimFinished = false;
    _float zOffset = 0.15f;
    _float m_fReleaseY = {};

    ENTER m_eEnter = ENTER::END;
};
NS_END
