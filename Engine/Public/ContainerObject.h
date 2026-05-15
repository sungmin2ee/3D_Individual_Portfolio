#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CContainerObject abstract : public CGameObject
{
public:
	typedef struct tagContainerObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{

	}CONTAINEROBJECT_DESC;
protected:
	CContainerObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CContainerObject(const CContainerObject& Prototype);
public:
	virtual ~CContainerObject();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	map<const _wstring, shared_ptr<class CPartObject>>		m_PartObjects;

protected:
	class CPartObject* Find_PartObject(const _wstring& strPartTag);
	class shared_ptr<CPartObject> Get_PartObject(const _wstring& strPartTag);
	HRESULT Add_PartObject(uint32_t iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg = nullptr);

public:
	virtual shared_ptr<CPrototype> Clone(void* pArg) = 0;
};

NS_END