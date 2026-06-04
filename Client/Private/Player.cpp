#include "Player.h"
#include "Body_Player.h"


#include "Weapon.h"
#include "GameInstance.h"

CPlayer::CPlayer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CContainerObject{ Prototype }
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
	PLAYER_DESC			Desc{};
	Desc.fSpeedPerSec = 0.1f;
	Desc.fRotationPerSec = 720.f;
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	auto pDesc = static_cast<PLAYER_DESC*>(pArg);
	m_eNextLevel = pDesc->nextLevel;

	m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-1.5f, 0, 0, 1));

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
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


		if (m_iState & CBody_Player::PLAYER_STATE::IDLE)
			m_iState ^= CBody_Player::PLAYER_STATE::IDLE;

		m_iState |= CBody_Player::PLAYER_STATE::RUN;
	}
	else
	{
		if (m_iState & CBody_Player::PLAYER_STATE::RUN)
			m_iState ^= CBody_Player::PLAYER_STATE::RUN;

		m_iState |= CBody_Player::PLAYER_STATE::IDLE;
	}*/
	__super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	
	CBody_Player::BODY_PLAYER_DESC		BodyDesc{};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pParentState = &m_iState;
	BodyDesc.fSpeedPerSec = 0.1f;
	BodyDesc.player = static_pointer_cast<CPlayer>(shared_from_this());
	BodyDesc.nextLevel = m_eNextLevel;
	if (FAILED(__super::Add_PartObject(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Body_Player"),
		TEXT("Part_Body"), &BodyDesc)))
		return E_FAIL;
	body = static_pointer_cast<CBody_Player>(__super::Get_PartObject(TEXT("Part_Body")));


	CWeapon::WEAPON_DESC		WeaponDesc{};
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	WeaponDesc.pParentState = &m_iState;
	WeaponDesc.pSocketMatrix = dynamic_pointer_cast<CBody_Player>(m_PartObjects[TEXT("Part_Body")])->Get_SocketMatrixPtr("rightPinky1");

	if (FAILED(__super::Add_PartObject(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Weapon"),
		TEXT("Part_Weapon"), &WeaponDesc)))
		return E_FAIL;
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

