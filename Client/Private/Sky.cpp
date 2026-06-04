#include "Sky.h"
#include "GameInstance.h"

CSky::CSky(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext }

{
}

CSky::CSky(const CSky& Prototype)
	: CGameObject{ Prototype }
{
}

CSky::~CSky()
{
	int a = 10;
}


HRESULT CSky::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSky::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);

	return S_OK;
}

void CSky::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSky::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(STATE::POSITION,
		XMLoadFloat4(CGameInstance::Get().Get_CamPosition()));

	__super::Update(fTimeDelta);
}

void CSky::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::PRIORITY, SHARED_THIS(CSky));

	__super::Late_Update(fTimeDelta);
}

HRESULT CSky::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", CGameInstance::Get().Get_Transform(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", CGameInstance::Get().Get_Transform(D3DTS::PROJ))))
		return E_FAIL;

	if (CGameInstance::Get().GetCurLevelIndex() == ETOUI(LEVEL::STAGE2)) {
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 1)))
			return E_FAIL;
	}
	else {
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;
	}


	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSky::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Cube>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Sky")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

unique_ptr<CSky> CSky::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CSky>(new CSky(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSky");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CSky::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CSky(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSky");
		return nullptr;
	}

	return pInstance;
}

