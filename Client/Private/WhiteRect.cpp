#include "WhiteRect.h"
#include "GameInstance.h"

CWhiteRect::CWhiteRect(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CWhiteRect::CWhiteRect(const CWhiteRect& Prototype)
	: CUIObject{ Prototype }
{
}

CWhiteRect::~CWhiteRect()
{
}


HRESULT CWhiteRect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWhiteRect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CWhiteRect::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CWhiteRect::Update(_float fTimeDelta)
{

	//m_bRender = true;
	if (m_bRender) {

		m_fAlpha += fTimeDelta;
	}

	if (m_fAlpha > 1.f) {
		m_fAlpha = 1.f;
	}
	__super::Update(fTimeDelta);
	
}

void CWhiteRect::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::ICON, SHARED_THIS(CWhiteRect));

	__super::Late_Update(fTimeDelta);
}

HRESULT CWhiteRect::Render()
{
	if (!m_bRender)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CWhiteRect::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_White")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

unique_ptr<CWhiteRect> CWhiteRect::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CWhiteRect>(new CWhiteRect(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWhiteRect");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CWhiteRect::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CWhiteRect(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWhiteRect");
		return nullptr;
	}

	return pInstance;
}

