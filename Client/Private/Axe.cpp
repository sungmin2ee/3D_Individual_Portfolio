#include "Axe.h"

#include "GameInstance.h"

CAxe::CAxe(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext }

{
}

CAxe::CAxe(const CAxe& Prototype)
	: CGameObject{ Prototype }
{
}

CAxe::~CAxe()
{

}


HRESULT CAxe::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAxe::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CAxe::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CAxe::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
}

void CAxe::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CAxe));

	__super::Late_Update(fTimeDelta);
}

HRESULT CAxe::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", CGameInstance::Get().Get_Transform(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", CGameInstance::Get().Get_Transform(D3DTS::PROJ))))
		return E_FAIL;



	uint32_t	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		m_pModelCom->Render(i);
	}



	return S_OK;
}

HRESULT CAxe::Ready_Components()
{
	//m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Axe")));
	//if (FAILED(__super::Add_Component(TEXT("Com_Model"), m_pModelCom)))
	//	return E_FAIL;
	//
	//m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxMesh")));
	//if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
	//	return E_FAIL;

	return S_OK;
}

unique_ptr<CAxe> CAxe::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CAxe>(new CAxe(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAxe");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CAxe::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CAxe(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAxe");
		return nullptr;
	}

	return pInstance;
}

