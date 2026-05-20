#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

//NS_BEGIN(Engine)
//class CShader;
//class CModel;
//class Obb;
//class VIBuffer_Collider;
//NS_END
NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END
NS_BEGIN(Client)

class CBlocker :
	public CGameObject
{
public:
	typedef struct tagBlockerDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_wstring leftTag;
		_wstring rightTag;
		_float4x4 worldMat;

	}BLOCKER_DESC;

private:
	CBlocker(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CBlocker(const CBlocker& Prototype);
public:
	virtual ~CBlocker();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Set_LeftDoor(shared_ptr<class CDoor> leftDoor) { m_pLeftDoor = leftDoor; }
	void Set_RightDoor(shared_ptr<class CDoor> rightDoor) { m_pRightDoor = rightDoor; }
	void Set_Tag(_wstring tag) { m_sTag = tag; }
	shared_ptr<class CDoor> Get_LeftDoor() { return m_pLeftDoor; }
	shared_ptr<class CDoor> Get_RightDoor() { return m_pRightDoor; }
	_wstring Get_Tag() { return m_sTag; }
	_wstring Get_LeftTag() { return m_sLeftTag; }
	_wstring Get_RightTag() { return m_sRightTag; }
private:
	HRESULT Ready_Components();
	void ExpandCollider();

private:
	shared_ptr<CShader>					m_pShaderCom = { nullptr };
	shared_ptr<CVIBuffer_Rect>			m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>				m_pTextureCom = { nullptr };
	//shared_ptr<CModel>					m_pModelCom = { nullptr };
	shared_ptr<Obb>						m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider>		m_pObbBfCom = { nullptr };
private:								
	shared_ptr<class CDoor>				m_pLeftDoor = { nullptr };
	shared_ptr<class CDoor>				m_pRightDoor = { nullptr };

	_wstring							m_sTag = L"";
	_wstring							m_sLeftTag = L"";
	_wstring							m_sRightTag = L"";

public:
	static unique_ptr<CBlocker> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};
NS_END
