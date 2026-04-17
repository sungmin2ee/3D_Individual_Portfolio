#pragma once


#include "Transform.h"
#include "GameInstance.h"
NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CPrototype
{
public:
	typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
	{
		const _tchar* pGameObjectTag;
	}GAMEOBJECT_DESC;

protected:
	CGameObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CGameObject(const CGameObject& Prototype);
public:
	virtual ~CGameObject() = default;
	
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	MODELOBJ_DESC& Get_Desc() { return m_ModelDesc; }
	CTransform* Get_Transform() { return m_pTransformCom.get(); }
	void Set_Dead() { m_bDead = true; }
	_bool Get_Dead() { return m_bDead; }

protected:
	HRESULT Add_Component(const _wstring& strComponentTag, shared_ptr<CComponent> pComponent);

protected:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };
	shared_ptr<class CTransform>	m_pTransformCom = { nullptr };
	shared_ptr<class CComponent> Find_Component(const _wstring& strComponentTag);

protected:
	MODELOBJ_DESC m_ModelDesc;
	_bool			m_bDead = false;
	map<const _wstring, shared_ptr<class CComponent>>		m_Components;
public:
	virtual shared_ptr<CPrototype> Clone(void* pArg) = 0;
};

NS_END