#pragma once

#include "Prototype.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CPrototype
{
protected:
	CComponent(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };
public:
	void SetOwner(shared_ptr<class CGameObject> pOwner) { m_pOwner = pOwner; }
	shared_ptr<class CGameObject > GetOwner() { return m_pOwner.lock(); }

public:
	virtual shared_ptr<CPrototype> Clone(void* pArg) = 0;
private:
	weak_ptr<CGameObject> m_pOwner;
};

NS_END