#pragma once
#include "Engine_Defines.h"
#include "Component.h"
#include "Shader.h"
#include "VIBuffer_Collider.h"
NS_BEGIN(Engine)
class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	CCollider(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CCollider();
public:

	virtual HRESULT Initialize();
	virtual bool Intersects(shared_ptr<CCollider> pTarget) = 0;
	virtual bool IntersectsRay(_float* pOutDist) = 0;
	void SetSelected(bool flag) { m_bIsSelected = flag; }
	virtual HRESULT Render() = 0;
	virtual void Update(_matrix TransformMatrix) = 0;
	bool GetSelected() { return m_bIsSelected; }
	//virtual void Update_Collider(_fmatrix WorldMatrix) = 0;
protected:
	bool m_bIsSelected = false;
	shared_ptr<CShader>  m_pShaderCom = nullptr;
	shared_ptr<VIBuffer_Collider> m_pBuffer = nullptr;


};
NS_END

