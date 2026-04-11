#pragma once
#include "Engine_Defines.h"
#include "Collider.h"

NS_BEGIN(Engine)
class ENGINE_DLL AABB :
	 public CCollider
{

private:
	AABB(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~AABB();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	const BoundingBox GetAABB() { return myAABB; }
	virtual HRESULT Render();
public:
	static unique_ptr<AABB> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
	void Update_AABB();
	void SetWorld(_float4x4 world) { m_WorldMatrix = world; }
	virtual bool Intersects(shared_ptr<CCollider> pTarget) override;
	virtual bool IntersectsRay(_float* pOutDist) override;
	void Set_WorldMatrix(_matrix matWorld) { XMStoreFloat4x4(&m_WorldMatrix, matWorld); }
	virtual void Update(_matrix TransformMatrix) override;


public:
	BoundingBox myAABB;
private:
	shared_ptr<class VIBuffer_Cube>					m_pBuffer = nullptr;

	_float4x4										m_WorldMatrix = {};

};
NS_END

