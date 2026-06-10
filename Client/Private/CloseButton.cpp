#include "CloseButton.h"

#include "GameInstance.h"
#include "Layer.h"
#include "FixUI.h"
#include "Level_Loading.h"


CCloseButton::CCloseButton(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CCloseButton::CCloseButton(const CCloseButton& Prototype)
	: CUIObject{ Prototype }
{
}

CCloseButton::~CCloseButton()
{
}


HRESULT CCloseButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCloseButton::Initialize(void* pArg)
{
	auto		pDesc = static_cast<CLOSEBUTTON_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(pDesc->nextLevel), TEXT("Layer_FixUI"));
	m_pFixUI = static_pointer_cast<CFixUI>(layer->GetObjectFirst());

	return S_OK;
}

void CCloseButton::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CCloseButton::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);

}

void CCloseButton::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CCloseButton));

	__super::Late_Update(fTimeDelta);
	if (m_pFixUI->Get_Render()) {
		m_bRender = true;
	}
	else {
		m_bRender = false;
	}
	if (m_pFixUI->Get_Render()) {
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_float minX, maxX, minY, maxY;
		minX = m_fX - (m_fSizeX * 0.5f);
		maxX = m_fX + (m_fSizeX * 0.5f);
		minY = m_fY - (m_fSizeY * 0.5f);
		maxY = m_fY + (m_fSizeY * 0.5f);

		if (ptMouse.x >= minX && ptMouse.x <= maxX && ptMouse.y >= minY && ptMouse.y <= maxY) {
			if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
				m_pFixUI->Set_Render(false);
			}
		}
	}

}

HRESULT CCloseButton::Render()
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

HRESULT CCloseButton::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Component_Texture_closeButton"));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}





unique_ptr<CCloseButton> CCloseButton::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CCloseButton>(new CCloseButton(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCloseButton");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CCloseButton::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CCloseButton(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCloseButton");
		return nullptr;
	}

	return pInstance;
}

