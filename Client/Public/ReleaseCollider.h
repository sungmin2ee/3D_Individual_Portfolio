#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class Obb;
class VIBuffer_Collider;
NS_END

NS_BEGIN(Client)

class CReleaseCollider final : public CGameObject
{
public:





private:
	CReleaseCollider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CReleaseCollider(const CReleaseCollider& Prototype);
public:
	virtual ~CReleaseCollider();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	shared_ptr<Obb> Get_Obb() { return m_pObbCom; };
private:
	shared_ptr<Obb> m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider> m_pObbBfCom = { nullptr };
private:
	HRESULT Ready_Components();
	void ExpandCollider();

public:
	static unique_ptr<CReleaseCollider> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

};

NS_END