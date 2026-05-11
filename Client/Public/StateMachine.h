#pragma once

#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "State.h"

NS_BEGIN(Client)

template <typename T> class State;


template <typename T>
class StateMachine {
private:
    // 생성자를 private에 두어 Create를 통해서만 생성하게 제한
    StateMachine() = default;

public:
    virtual ~StateMachine() = default;

    // 클래스 내부 정의 시 template<typename T>를 중복해서 붙이지 않습니다.
    HRESULT Initialize(T* owner, shared_ptr<State<T>> initialState) {
        if (!owner || !initialState)
            return E_FAIL;

        m_pOwner = owner;
        m_pCurrentState = initialState;
        m_pCurrentState->Enter(*m_pOwner);

        return S_OK;
    }

    void ChangeState(shared_ptr<State<T>> pNewState) {
        if (m_pCurrentState == pNewState || pNewState == nullptr)
            return;

        if (m_pCurrentState) {
            m_pCurrentState->Exit(*m_pOwner);
        }

        m_pCurrentState = pNewState;

        if (m_pCurrentState) {
            m_pCurrentState->Enter(*m_pOwner);
        }
    }

    void Update(_float fTimeDelta) {
        if (m_pCurrentState && m_pOwner) {
            m_pCurrentState->Update(*m_pOwner, fTimeDelta);
        }
    }


public:
    static unique_ptr<StateMachine<T>> Create(T* owner, shared_ptr<State<T>> initialState) {
        auto pInstance = unique_ptr<StateMachine<T>>(new StateMachine<T>());
        if (FAILED(pInstance->Initialize(owner, initialState))) {
            return nullptr;
        }
        return pInstance;
    }

private:
    T* m_pOwner = nullptr;
    shared_ptr<State<T>> m_pCurrentState = nullptr;
};

NS_END