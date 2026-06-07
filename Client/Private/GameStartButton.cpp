#include "GameStartButton.h"

#include "GameInstance.h"
#include "Layer.h"
#include "MapInfo.h"
#include "Level_Loading.h"


CGameStartButton::CGameStartButton(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CGameStartButton::CGameStartButton(const CGameStartButton& Prototype)
	: CUIObject{ Prototype }
{
}

CGameStartButton::~CGameStartButton()
{
}


HRESULT CGameStartButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameStartButton::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_bRender = true;


	return S_OK;
}

void CGameStartButton::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CGameStartButton::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CGameStartButton::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CGameStartButton));

	__super::Late_Update(fTimeDelta);
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
			CGameInstance::Get().Get_CurrentLevel()->Set_ChangeLevel();
			CGameInstance::Get().Get_CurrentLevel()->Set_NextLevel((ETOUI(LEVEL::SHELTER)));
		}
	}
}

HRESULT CGameStartButton::Render()
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

HRESULT CGameStartButton::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::LOGO), L"Prototype_Component_Texture_GameStart"));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}





unique_ptr<CGameStartButton> CGameStartButton::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CGameStartButton>(new CGameStartButton(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameStartButton");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CGameStartButton::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CGameStartButton(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameStartButton");
		return nullptr;
	}

	return pInstance;
}

