#include "Weapon.h"
#include "Layer.h"
#include "Player.h"
#include "Body_Player.h"

#include "GameInstance.h"

CWeapon::CWeapon(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CPartObject{ pDevice, pContext }

{
}

CWeapon::CWeapon(const CWeapon& Prototype)
	: CPartObject{ Prototype }
{
}

CWeapon::~CWeapon()
{

}


HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	auto		pDesc = static_cast<CWeapon::WEAPON_DESC*>(pArg);

	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(0.7f, 0.7f, 0.7f);
	m_pTransformCom->Rotation(XMVectorSet(1.f, 1.f, 1.f, 0.f), 120.f);



	m_pTransformCom->Set_State(STATE::POSITION,
		XMVectorSet(
			0.f,
			0.05f,
			0.2f,
			1.f
		));


	return S_OK;
}

void CWeapon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CWeapon::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
}

void CWeapon::Late_Update(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);


	_matrix		ChildMatrix = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * SocketMatrix;

	Make_CombinedWorldMatrix(ChildMatrix);

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CWeapon));

	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon::Render()
{
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
	if (layer == nullptr)
		return E_FAIL;
	auto player = layer->GetObjectFirst();
	if (player == nullptr)
		return E_FAIL;
	auto pPlayer  = static_pointer_cast<CPlayer>(player);
	if (pPlayer->Get_Body()->Get_CurState() == CBody_Player::PLAYER_STATE::ATTACK && pPlayer->Get_Body()->Get_Weapon() == CBody_Player::PLAYER_WEAPON::AXE) {
		if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
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

	}
	

	return S_OK;
}

HRESULT CWeapon::Ready_Components()
{
	m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Model_Axe")));
	if (FAILED(__super::Add_Component(TEXT("Com_Model"), m_pModelCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

unique_ptr<CWeapon> CWeapon::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CWeapon>(new CWeapon(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CWeapon::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CWeapon(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon");
		return nullptr;
	}

	return pInstance;
}

