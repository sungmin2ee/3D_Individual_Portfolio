#include "GameObject.h"

#include "Transform.h"

CGameObject::CGameObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pDevice{ Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
{
}


HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (nullptr == pArg)
		return S_OK;

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	auto		pDesc = static_cast<MODELOBJ_DESC*>(pArg);
	m_ModelDesc.pModelPrototypeTag = pDesc->pModelPrototypeTag;
	m_ModelDesc.pShaderPrototypeTag = pDesc->pShaderPrototypeTag;
	m_ModelDesc.levelIndex = pDesc->levelIndex;
	m_ModelDesc.filePath = pDesc->filePath;
	m_ModelDesc.collide = pDesc->collide;
	m_ModelDesc.worldMatrix = pDesc->worldMatrix;
	//lstrcpy(m_szName, pDesc->pModelPrototypeTag);


	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
	int a = 10;
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(const _wstring& strComponentTag, shared_ptr<CComponent> pComponent)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);
	return S_OK;
}

shared_ptr<class CComponent> CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}