#include "Fade.h"

#include "GameInstance.h"

CFade::CFade(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CFade::CFade(const CFade& Prototype)
	: CUIObject{ Prototype }
{
}

CFade::~CFade()
{
}


HRESULT CFade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CFade::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CFade::Update(_float fTimeDelta)
{
	if (m_bReset) {
		Reset();
	}
	if (fadeOut) {
		falpha += fTimeDelta;
		if (falpha > 1.f) {
			falpha = 1.f;
			fadeOut = false;
			m_bFinished = true;
		}
	}
	if (fadeIn) {
		falpha -= fTimeDelta;
		if (falpha < 0.f) {
			falpha = 0.f;
			fadeIn = false;
			m_bFinished = true;
		}
	}
	__super::Update(fTimeDelta);

}

void CFade::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::ICON, SHARED_THIS(CFade));

	__super::Late_Update(fTimeDelta);
}

HRESULT CFade::Render()
{
	//if (!m_bRender)
	//	return E_FAIL;
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("fAlpha", &falpha,sizeof(_float))))
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

HRESULT CFade::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_FadeInOut")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Black")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

void CFade::Reset()
{
	if (fadeIn) {
		falpha = 1.f;
		
	}
	if (fadeOut) {
		falpha = 0.f;
	}
	m_bReset = false;
}

unique_ptr<CFade> CFade::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CFade>(new CFade(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFade");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CFade::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CFade(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFade");
		return nullptr;
	}
	
	return pInstance;
}

