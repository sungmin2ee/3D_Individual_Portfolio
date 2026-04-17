#include "Inventory.h"

#include "GameInstance.h"

CInventory::CInventory(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CInventory::CInventory(const CInventory& Prototype)
	: CUIObject{ Prototype }
{
}

CInventory::~CInventory()
{
	int a = 10;
}


HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	auto		pDesc = static_cast<BACKGROUND_DESC*>(pArg);


	pDesc->pGameObjectTag = TEXT("Invnetory_Frame");
	pDesc->fSpeedPerSec = 0.f;
	pDesc->fRotationPerSec = 0.f;


	pDesc->fSizeX = g_iWinSizeX * 0.35f;
	pDesc->fSizeY = g_iWinSizeY *0.75f;

	pDesc->fX = g_iWinSizeX * 0.5f;
	pDesc->fY = g_iWinSizeY - pDesc->fSizeY * 0.5f;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CInventory::Update(_float fTimeDelta)
{
	/*m_fX += 20.f * fTimeDelta;
	m_fSizeY += 100.f * fTimeDelta;*/

	__super::Update(fTimeDelta);
}

void CInventory::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CInventory));

	__super::Late_Update(fTimeDelta);
}

HRESULT CInventory::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

unique_ptr<CInventory> CInventory::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CInventory>(new CInventory(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CInventory::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CInventory(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory");
		return nullptr;
	}

	return pInstance;
}

