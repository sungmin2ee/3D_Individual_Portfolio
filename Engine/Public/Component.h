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
	void SetOwner(class CGameObject* pOwner) { m_pOwner = pOwner; }
	CGameObject* GetOwner() { return m_pOwner; }

public:
	virtual shared_ptr<CPrototype> Clone(void* pArg) = 0;
private:
	CGameObject* m_pOwner = nullptr;
};

NS_END