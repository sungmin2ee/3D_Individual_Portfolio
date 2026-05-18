#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class Obb;
class VIBuffer_Collider;
NS_END

NS_BEGIN(Client)

class CStair_Collider final : public CGameObject
{
public:
	enum class STAIR_COLLIDER {
		STAIR_UP, STAIR_DOWN, END
	};

	typedef struct tagStairDesc: public GAMEOBJECT_DESC
	{
		STAIR_COLLIDER state;
	}STAIR_DESC;



private:
	CStair_Collider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CStair_Collider(const CStair_Collider& Prototype);
public:
	virtual ~CStair_Collider();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<Obb> m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider> m_pObbBfCom = { nullptr };
private:
	HRESULT Ready_Components();
	void ExpandCollider();

public:
	static unique_ptr<CStair_Collider> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	STAIR_COLLIDER m_eState = STAIR_COLLIDER::END;
};

NS_END