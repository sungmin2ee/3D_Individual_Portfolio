#include "FixUI.h"

#include "GameInstance.h"
#include "Layer.h"

CFixUI::CFixUI(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CFixUI::CFixUI(const CFixUI& Prototype)
	: CUIObject{ Prototype }
{
}

CFixUI::~CFixUI()
{
}


HRESULT CFixUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFixUI::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	auto		pDesc = static_cast<MAPINFO_DESC*>(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CFixUI::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CFixUI::Update(_float fTimeDelta)
{
	//m_bRender = true;

	auto items = CGameInstance::Get().Get_Items();
	//Prototype_Component_Texture_icon_lumber
	//Prototype_Component_Texture_icon_connectors
	for (auto& item : items) {
		if (item.first == L"Prototype_Component_Texture_icon_lumber") {
			m_iLumber = item.second;
		}
		else if (item.first == L"Prototype_Component_Texture_icon_connectors") {
			m_iConnector = item.second;
		}
	}

	__super::Update(fTimeDelta);

}

void CFixUI::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CFixUI));

	__super::Late_Update(fTimeDelta);
}

HRESULT CFixUI::Render()
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


	CGameInstance::Get().RenderText(1, L"¼ö¸®", (m_fX * 0.95f), (m_fY *0.55f), DirectX::Colors::White, 0.9f);
	_wstring lumbQuantity = to_wstring(m_iLumber) + L"/" + to_wstring(2);
	_wstring connQuantity = to_wstring(m_iConnector) + L"/" + to_wstring(2);

	if (m_iLumber >= 2) {
		CGameInstance::Get().RenderText(1, lumbQuantity, (m_fX * 0.85f), (m_fY), DirectX::Colors::White, 0.9f);
	}
	else {
		CGameInstance::Get().RenderText(1, lumbQuantity, (m_fX * 0.85f), (m_fY), DirectX::Colors::Red, 0.9f);
	}
	if (m_iConnector >= 2) {
		CGameInstance::Get().RenderText(1, connQuantity, (m_fX * 1.05f), (m_fY), DirectX::Colors::White, 0.9f);
	}
	else {
		CGameInstance::Get().RenderText(1, connQuantity, (m_fX * 1.05f), (m_fY), DirectX::Colors::Red, 0.9f);
	}


	return S_OK;
}

HRESULT CFixUI::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_door_fix_ui")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}



unique_ptr<CFixUI> CFixUI::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CFixUI>(new CFixUI(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFixUI");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CFixUI::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CFixUI(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFixUI");
		return nullptr;
	}

	return pInstance;
}

