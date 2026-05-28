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
	//ZOMBIE_DESC			Desc{};
	//Desc.fSpeedPerSec = 10.f;
	//Desc.fRotationPerSec = 180.f;

	auto		pDesc = static_cast<ZOMBIE_DESC*>(pArg);



	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	
	if (FAILED(Ready_PartObjects(pArg)))
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
	
	__super::Update(fTimeDelta);


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

HRESULT CZombie::Ready_PartObjects(void *pArg)
{
	
	auto		pDesc = static_cast<ZOMBIE_DESC*>(pArg);



	CBody_Zombie::BODY_ZOMBIE_DESC		BodyDesc{};
	BodyDesc.pParentState = &m_iState;
	BodyDesc.fSpeedPerSec = pDesc->fSpeedPerSec;
	BodyDesc.firstState = pDesc->firstState;
	BodyDesc.State = pDesc->State;
	BodyDesc.Direction = pDesc->Direction;
	BodyDesc.fRotationPerSec = pDesc->fRotationPerSec;
	BodyDesc.pos = pDesc->pos;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
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

