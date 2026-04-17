#include "Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: ECamera{ pDevice, pContext }

{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: ECamera{ Prototype }
{
}

CCamera_Free::~CCamera_Free()
{
	int a = 10;
}


HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	auto	pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	__super::Update_PipeLine();

	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{


	__super::Update(fTimeDelta);
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{


	__super::Late_Update(fTimeDelta);
}

HRESULT CCamera_Free::Render()
{

	return S_OK;
}

HRESULT CCamera_Free::Ready_Components()
{



	return S_OK;
}

unique_ptr<CCamera_Free> CCamera_Free::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CCamera_Free>(new CCamera_Free(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CCamera_Free::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CCamera_Free(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		return nullptr;
	}

	return pInstance;
}

