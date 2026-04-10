#pragma once
#include "Engine_Defines.h"
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	CCollider(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CCollider();

	virtual bool Intersects(shared_ptr<CCollider> pTarget) = 0;
	virtual bool IntersectsRay() = 0;
	//virtual void Update_Collider(_fmatrix WorldMatrix) = 0;
};
NS_END

