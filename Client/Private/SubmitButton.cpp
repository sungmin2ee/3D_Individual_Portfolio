#include "SubmitButton.h"

#include "GameInstance.h"
#include "Layer.h"
#include "FixUI.h"
#include "Level_Loading.h"
#include "Bomb.h"


CSubmitButton::CSubmitButton(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CSubmitButton::CSubmitButton(const CSubmitButton& Prototype)
	: CUIObject{ Prototype }
{
}

CSubmitButton::~CSubmitButton()
{
}


HRESULT CSubmitButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSubmitButton::Initialize(void* pArg)
{
	auto		pDesc = static_cast<SUBMIT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(pDesc->nextLevel), TEXT("Layer_FixUI"));
	m_pFixUI = static_pointer_cast<CFixUI>(layer->GetObjectFirst());

	return S_OK;
}

void CSubmitButton::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CSubmitButton::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);

}

void CSubmitButton::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CSubmitButton));

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
				m_bButtonClicked = true;
				m_pFixUI->Set_Render(false);
			}
		}
	}
	if (m_bButtonClicked) {
		m_fTime += fTimeDelta;
		if (m_fTime > 3.f) {
			auto NuclearLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Nuclear");
			if (NuclearLayer == nullptr) return;
			auto objects = NuclearLayer->GetObjects();
			for (auto& obj : objects) {
				if (obj->Get_Tag() == L"Bomb") {
					static_pointer_cast<CBomb>(obj)->Set_Start();
					m_bButtonClicked = false;
					break;
				}
			}
		}
	}

}

HRESULT CSubmitButton::Render()
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

HRESULT CSubmitButton::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Component_Texture_SubmitButton"));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}





unique_ptr<CSubmitButton> CSubmitButton::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CSubmitButton>(new CSubmitButton(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSubmitButton");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CSubmitButton::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CSubmitButton(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSubmitButton");
		return nullptr;
	}

	return pInstance;
}

