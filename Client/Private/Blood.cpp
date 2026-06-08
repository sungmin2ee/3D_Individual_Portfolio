#include "Blood.h"

#include "GameInstance.h"
#include "Zombie.h"
#include "Body_Zombie.h"

CBlood::CBlood(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CPartObject{ pDevice, pContext }

{
}

CBlood::CBlood(const CBlood& Prototype)
	: CPartObject{ Prototype }
{
}

CBlood::~CBlood()
{

}


HRESULT CBlood::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlood::Initialize(void* pArg)
{
 	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	auto	pDesc = static_cast<BLOOD_DESC*>(pArg);
	m_pZombie = pDesc->zombie;
	/*m_pTransformCom->Set_State(STATE::POSITION,
		XMVectorSet(64.5f, 20.f, 64.5f, 1.f));*/

	return S_OK;
}

void CBlood::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBlood::Update(_float fTimeDelta)
{
	Make_CombinedWorldMatrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	if (m_bBleeding) {
		m_pVIBufferCom->Blood(fTimeDelta);

	}

	__super::Update(fTimeDelta);
}

void CBlood::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONLIGHT, SHARED_THIS(CBlood));

	__super::Late_Update(fTimeDelta);
}

HRESULT CBlood::Render()
{
	if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", CGameInstance::Get().Get_Transform(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", CGameInstance::Get().Get_Transform(D3DTS::PROJ))))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", CGameInstance::Get().Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	
	
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	
	return S_OK;
}

void CBlood::Play_Particle()
{
	auto headMat = XMLoadFloat4x4(static_pointer_cast<CZombie>(m_pZombie.lock())->Get_Body()->Get_HeadMat());
	_vector headPos = headMat.r[3];
	_float3 myPos;
	XMStoreFloat3(&myPos, headPos);
	m_pVIBufferCom->Set_Center(myPos);
	m_pVIBufferCom->Reset();
}

HRESULT CBlood::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Particle_Point>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Particle_Blood")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_Particle_Point")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Blood")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

unique_ptr<CBlood> CBlood::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CBlood>(new CBlood(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlood");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CBlood::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CBlood(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlood");
		return nullptr;
	}

	return pInstance;
}

