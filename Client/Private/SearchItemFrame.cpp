#include "SearchItemFrame.h"

#include "GameInstance.h"
#include "Layer.h"
#include "SearchBox.h"


CSearchItemFrame::CSearchItemFrame(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CSearchItemFrame::CSearchItemFrame(const CSearchItemFrame& Prototype)
	: CUIObject{ Prototype }
{
}

CSearchItemFrame::~CSearchItemFrame()
{

}


HRESULT CSearchItemFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSearchItemFrame::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSearchItemFrame::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSearchItemFrame::Update(_float fTimeDelta)
{
	/*m_fX += 20.f * fTimeDelta;
	m_fSizeY += 100.f * fTimeDelta;*/

	__super::Update(fTimeDelta);
	//if (CGameInstance::Get().Key_Down(DIK_E)) {
	//	m_bRender = !m_bRender;
	//}

	auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_SearchBox"));
	auto searchBox = layer->GetObjectFirst();

	if(static_pointer_cast<CSearchBox>(searchBox)->Get_Render()){
		m_bRender = true;
	}
	else {
		m_bRender = false;

	}



}

void CSearchItemFrame::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CSearchItemFrame));

	__super::Late_Update(fTimeDelta);
}

HRESULT CSearchItemFrame::Render()
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

HRESULT CSearchItemFrame::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_smaller_item_border")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

unique_ptr<CSearchItemFrame> CSearchItemFrame::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CSearchItemFrame>(new CSearchItemFrame(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSearchItemFrame");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CSearchItemFrame::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CSearchItemFrame(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSearchItemFrame");
		return nullptr;
	}

	return pInstance;
}

