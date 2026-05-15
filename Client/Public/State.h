#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

NS_BEGIN(Client)

template <typename T>
class State {
public:
    State() {}
    State(_float fVelocity) {}
    virtual ~State() = default;

    // T는 Player가 될 수도, Monster가 될 수도 있습니다.
    virtual void Enter(T& owner) = 0;
    virtual void Update(T& owner, _float deltaTime) = 0;
    virtual void Exit(T& owner) = 0;

};

NS_END