#include "BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject { pDevice, pContext }

{
}

CBackGround::CBackGround(const CBackGround& Prototype)
	: CGameObject{ Prototype }
{
}

CBackGround::~CBackGround()
{

}


HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	auto		pDesc = static_cast<BACKGROUND_DESC*>(pArg);
	m_iData = pDesc->iData;

	pDesc->pGameObjectTag = TEXT("BackGround");
	pDesc->fSpeedPerSec = 10.f;
	pDesc->fRotationPerSec = 180.f;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	/*BACKGROUND_DESC			Desc{};

	Desc.pGameObjectTag = TEXT("BackGround");
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = 180.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;*/

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CBackGround));
}

HRESULT CBackGround::Render()
{
	return S_OK;
}

unique_ptr<CBackGround> CBackGround::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CBackGround>(new CBackGround(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CBackGround::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CBackGround(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		return nullptr;
	}

	return pInstance;
}

