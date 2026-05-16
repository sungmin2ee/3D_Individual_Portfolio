#include "Zombie.h"


#include "GameInstance.h"

CZombie::CZombie(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CZombie::CZombie(const CZombie& Prototype)
	: CContainerObject{ Prototype }
{
}

CZombie::~CZombie()
{
}

HRESULT CZombie::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CZombie::Initialize(void* pArg)
{
	ZOMBIE_DESC			Desc{};
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = 180.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
	return S_OK;
}

void CZombie::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CZombie::Update(_float fTimeDelta)
{
	/*if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);

		
		if (m_iState & CBody_Zombie::PLAYER_STATE::IDLE)
			m_iState ^= CBody_Zombie::PLAYER_STATE::IDLE;

		m_iState |= CBody_Zombie::PLAYER_STATE::RUN;
	}
	else
	{
		if (m_iState & CBody_Zombie::PLAYER_STATE::RUN)
			m_iState ^= CBody_Zombie::PLAYER_STATE::RUN;

		m_iState |= CBody_Zombie::PLAYER_STATE::IDLE;
	}*/
	__super::Update(fTimeDelta);

	if (body->Get_HP() <= 0) {
		m_bDead = true;
	}
}

void CZombie::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CZombie::Render()
{
	return S_OK;
}

HRESULT CZombie::Ready_Components()
{
	return S_OK;
}

HRESULT CZombie::Ready_PartObjects()
{
	CBody_Zombie::BODY_ZOMBIE_DESC		BodyDesc{};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pParentState = &m_iState;
	BodyDesc.fSpeedPerSec = 0.1f;
	if (FAILED(__super::Add_PartObject(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Body_Zombie"),
		TEXT("Part_Body"), &BodyDesc)))
		return E_FAIL;
	body = static_pointer_cast<CBody_Zombie>(__super::Get_PartObject(TEXT("Part_Body")));

	return S_OK;
}

unique_ptr<CZombie> CZombie::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CZombie>(new CZombie(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CZombie");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CZombie::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CZombie(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CZombie");
		return nullptr;
	}

	return pInstance;
}

