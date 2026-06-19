#include "Bomb.h"
#include "Nuclear.h"
#include "Layer.h"

#include "GameInstance.h"

CBomb::CBomb(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext }

{
}

CBomb::CBomb(const CBomb& Prototype)
	: CGameObject{ Prototype }
{
}

CBomb::~CBomb()
{

}


HRESULT CBomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBomb::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_Scale(0.002f, 0.002f, 0.002f);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), -90);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-2.5f, 5.f, 3.f, 1.f));
	return S_OK;
}

void CBomb::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CBomb::Update(_float fTimeDelta)
{
	if (!m_bStart)
		return;
	__super::Update(fTimeDelta);

	_float4 myPos;
	XMStoreFloat4(&myPos, m_pTransformCom->Get_State(STATE::POSITION));


	if (myPos.y < 0) {
		auto NuclearLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Nuclear");
		if (NuclearLayer == nullptr) return;
		auto objects = NuclearLayer->GetObjects();
		for (auto& obj : objects) {
			if (obj->Get_Tag() == L"Nuclear") {
				static_pointer_cast<CNuclear>(obj)->Set_Start();
				break;
			}
		}
		m_bDead = true;
	}

	myPos.y -= (fTimeDelta * 4.f);
	m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&myPos));

}

void CBomb::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CBomb));

	__super::Late_Update(fTimeDelta);
}

HRESULT CBomb::Render()
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

HRESULT CBomb::Ready_Components()
{
	m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Model_Bomb")));

	
	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh")));

	return S_OK;
}

unique_ptr<CBomb> CBomb::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CBomb>(new CBomb(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBomb");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CBomb::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CBomb(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBomb");
		return nullptr;
	}

	return pInstance;
}

