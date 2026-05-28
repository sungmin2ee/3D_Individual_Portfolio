#pragma once

#include "State.h"
#include "Body_Zombie.h"
NS_BEGIN(Client)
class CZombie_Stair :
    public State<CBody_Zombie>
{

public:
    enum STAIR_STATE { DOWN, DOWNFAST,UP, UPFAST, IDLE_DOWN, IDLE_UP,END };
private:
    CZombie_Stair();

public:
    virtual ~CZombie_Stair();
public:
    virtual void Enter(CBody_Zombie& owner) override;
    virtual void Update(CBody_Zombie& owner, _float deltaTime) override;
    virtual void Exit(CBody_Zombie& owner) override;

public:
    static unique_ptr<CZombie_Stair> Create();

    void CheckExit(CBody_Zombie& owner, _float4 myPos);


    _bool animStart = false;
    STAIR_STATE m_eCurstate = IDLE_UP;
    STAIR_STATE m_ePrestate = END;
    _float zOffset = 0.15f;
    _float m_fReleaseY = {};
    _bool m_bAdjustingEnter = false;
    _bool m_bAdjustingExit = false;
    _bool m_bExiting = false;
    _bool m_bEnterAnimFinished = false;
};
NS_END
