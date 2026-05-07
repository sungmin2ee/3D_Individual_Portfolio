#include "Player.h"
#include "CModel.h"
#include "GameInstance.h"
#include "Obb.h"
#include "GameObject.h"
CPlayer::CPlayer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext }

{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject{ Prototype }
{
}

CPlayer::~CPlayer()
{
}


HRESULT CPlayer::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{




	auto		pDesc = static_cast<PLAYER_DESC*>(pArg);



	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Calculate_Box(ETOUI(MODEL::ANIM));


	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(this);
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CPlayer::Update(_float fTimeDelta)
{
	//if (CGameInstance::Get().Key_Down(DIKEYBOARD_6)) {
	//	m_pTransformCom->Set_State(STATE::POSITION,XMVectorSet(5,5,5,1));
	//}
	m_pModelCom->Play_Animation(fTimeDelta);

	m_pModelCom->Calculate_Box(ETOUI(MODEL::ANIM));
	__super::Update(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(CPlayer));
}

HRESULT CPlayer::Render()
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
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		m_pModelCom->Render(i);
	}

	m_pObbCom->Render();
	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	m_pModelCom = static_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_Model_Joe"));
	if (nullptr == m_pModelCom)
	{
		MSG_BOX("Player의 모델 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pShaderCom = static_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Component_Shader_VtxAnimMesh"));
	if (nullptr == m_pShaderCom)
	{
		MSG_BOX("쉐이더 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pObbBfCom = static_pointer_cast<VIBuffer_Cube>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Cube_Buffer"));
	if (nullptr == m_pObbBfCom)
	{
		MSG_BOX("OBB 버퍼 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pObbCom = static_pointer_cast<Obb>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_OBB", &m_pObbBfCom));
	if (nullptr == m_pObbCom)
	{
		MSG_BOX("OBB 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	return S_OK;
}
unique_ptr<CPlayer> CPlayer::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CPlayer>(new CPlayer(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CPlayer::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CPlayer(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		return nullptr;
	}

	return pInstance;
}

