#include "SearchItemIcon.h"

#include "GameInstance.h"
#include "SearchBox.h"
#include "Layer.h"

CSearchItemIcon::CSearchItemIcon(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CSearchItemIcon::CSearchItemIcon(const CSearchItemIcon& Prototype)
	: CUIObject{ Prototype }
{
}

CSearchItemIcon::~CSearchItemIcon()
{
	int a = 10;
}


HRESULT CSearchItemIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSearchItemIcon::Initialize(void* pArg)
{
	auto pDesc = static_cast<ITEMICON_DESC*>(pArg);
	itemIconTag = pDesc->pGameObjectTag;
	m_bRender = pDesc->bInitialRender;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(itemIconTag)))
		return E_FAIL;

	return S_OK;
}

void CSearchItemIcon::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CSearchItemIcon::Update(_float fTimeDelta)
{


	__super::Update(fTimeDelta);
	//if (CGameInstance::Get().Key_Down(DIK_E)) {
	//	m_bRender = !m_bRender;
	//}

	auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_SearchBox"));
	auto searchBox = layer->GetObjectFirst();

	if (static_pointer_cast<CSearchBox>(searchBox)->Get_Render()) {
		m_bRender = true;
	}
	else {
		m_bRender = false;

	}
}

void CSearchItemIcon::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::ICON, SHARED_THIS(CSearchItemIcon));

	__super::Late_Update(fTimeDelta);
}

HRESULT CSearchItemIcon::Render()
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

HRESULT CSearchItemIcon::Ready_Components(_wstring iconTag)
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), iconTag));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

unique_ptr<CSearchItemIcon> CSearchItemIcon::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CSearchItemIcon>(new CSearchItemIcon(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSearchItemIcon");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CSearchItemIcon::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CSearchItemIcon(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSearchItemIcon");
		return nullptr;
	}
	
	return pInstance;
}

