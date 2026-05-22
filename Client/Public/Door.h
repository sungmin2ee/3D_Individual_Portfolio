#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class Obb;
class VIBuffer_Collider;
NS_END

NS_BEGIN(Client)

class CDoor :
    public CGameObject
{
public:
	typedef struct tagDoorDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_wstring leftTag;
		_wstring rightTag;
		_float4x4 worldMat;

	}DOOR_DESC;

private:
	CDoor(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CDoor(const CDoor& Prototype);
public:
	virtual ~CDoor();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_LeftBlocker(shared_ptr<class CBlocker> leftBlocker) { m_pLeftBlocker = leftBlocker; }
	void Set_RightBlocker(shared_ptr<class CBlocker> rightBlocker) { m_pRightBlocker = rightBlocker; }
	shared_ptr<class CBlocker> Get_LeftBlocker() { return m_pLeftBlocker.lock(); }
	shared_ptr<class CBlocker> Get_RightBlocker() { return m_pRightBlocker.lock(); }
	void Set_Tag(_wstring tag) { m_sTag = tag; }
	void Set_DoorOpened(_bool flag) { m_bDoorOpened = flag; }
	_wstring Get_Tag() { return m_sTag; }
	_wstring Get_LeftTag() { return m_sLeftTag; }
	_wstring Get_RightTag() { return m_sRightTag; }
	_bool Get_DoorOpened() { return m_bDoorOpened; }
	shared_ptr<Obb>	 Get_Obb() { return m_pObbCom; }
private:
	HRESULT Ready_Components();
	void ExpandCollider();

private:
	shared_ptr<CShader>				m_pShaderCom = { nullptr };
	shared_ptr<CModel>				m_pModelCom = { nullptr };
	shared_ptr<Obb>					m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider>		m_pObbBfCom = { nullptr };

private:
	weak_ptr<class CBlocker>		m_pLeftBlocker;
	weak_ptr<class CBlocker>		m_pRightBlocker;

	_wstring						m_sTag = L"";
	_wstring						m_sLeftTag = L"";
	_wstring						m_sRightTag = L"";
	_bool							m_bDoorOpened = false;
public:
	static unique_ptr<CDoor> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};
NS_END
