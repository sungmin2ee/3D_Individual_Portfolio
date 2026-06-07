#include "MapInfo.h"

#include "GameInstance.h"
#include "Layer.h"

CMapInfo::CMapInfo(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CMapInfo::CMapInfo(const CMapInfo& Prototype)
	: CUIObject{ Prototype }
{
}

CMapInfo::~CMapInfo()
{
}


HRESULT CMapInfo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapInfo::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	auto		pDesc = static_cast<MAPINFO_DESC*>(pArg);

	if (FAILED(Ready_Components(pDesc->textureTag)))
		return E_FAIL;

	return S_OK;
}

void CMapInfo::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CMapInfo::Update(_float fTimeDelta)
{
	//m_bRender = true;

	__super::Update(fTimeDelta);

}

void CMapInfo::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CMapInfo));

	__super::Late_Update(fTimeDelta);
}

HRESULT CMapInfo::Render()
{
	if (!m_bRender)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapInfo::Ready_Components(_wstring textrueTag)
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER), textrueTag));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}



unique_ptr<CMapInfo> CMapInfo::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CMapInfo>(new CMapInfo(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapInfo");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CMapInfo::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CMapInfo(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapInfo");
		return nullptr;
	}

	return pInstance;
}

