#pragma once
#include "Engine_Defines.h"
#include "Collider.h"
NS_BEGIN(Engine)
class ENGINE_DLL Obb : public CCollider 
{

private:
	Obb(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~Obb();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	const BoundingOrientedBox GetObb() { return myOBB; }
	virtual HRESULT Render();
public:
	static unique_ptr<Obb> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
	void Update_OBB();
	void SetWorld(_float4x4 world){ m_WorldMatrix = world;}
	virtual bool Intersects(shared_ptr<CCollider> pTarget) override;
	virtual bool IntersectsRay(_float* pOutDist) override;
	void SetBuffer(shared_ptr<class VIBuffer_Cube> bf) { m_pBuffer = bf; }
	void Set_WorldMatrix(_matrix matWorld) { XMStoreFloat4x4(&m_WorldMatrix, matWorld); }
public:
	BoundingOrientedBox myOBB;

private:
	shared_ptr<class VIBuffer_Cube>					m_pBuffer = nullptr;
	_float4x4										m_WorldMatrix = {};

};
NS_END

