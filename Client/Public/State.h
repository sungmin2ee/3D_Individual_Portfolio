#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

NS_BEGIN(Client)

template <typename T>
class State {
public:
    State() : m_fVelocity(0.f) {}
    State(_float fVelocity) : m_fVelocity(fVelocity) {}
    virtual ~State() = default;

    // T는 Player가 될 수도, Monster가 될 수도 있습니다.
    virtual void Enter(T& owner) = 0;
    virtual void Update(T& owner, _float deltaTime) = 0;
    virtual void Exit(T& owner) = 0;

protected:
    _float m_fVelocity = 0.f;
};

NS_END