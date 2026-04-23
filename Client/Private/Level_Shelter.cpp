#include "Level_Logo.h"
#include "GameInstance.h"
#include "Camera_Free.h"

#include "Level_Shelter.h"
#include "Inventory.h"

CLevel_Shelter::CLevel_Shelter(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }, m_pDevice{ pDevice }, m_pContext{ pContext }
{
}

CLevel_Shelter::~CLevel_Shelter()
{
}

HRESULT CLevel_Shelter::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;



	return S_OK;
}

void CLevel_Shelter::Update(_float fTimeDelta)
{

	if (CGameInstance::Get().Get_DIKeyState(DIK_CAPITAL)) {
		CGameInstance::Get().Save(ETOUI(LEVEL::SHELTER));
	}

}

HRESULT CLevel_Shelter::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("쉘터레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CInventory::INVENTORY_DESC pDesc;

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Inventory"),
		ETOUI(LEVEL::SHELTER), strLayerTag,&pDesc)))
		return E_FAIL;


	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_Inventory", ETOUI(LEVEL::SHELTER), strLayerTag)))
	//	return E_FAIL;
	
	//CGameInstance::Get().Load(ETOUI(LEVEL::SHELTER));


	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC		FreeDesc{};
	FreeDesc.vEye = _float4(0.f, 10.f, -5.f, 1.f);
	FreeDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	FreeDesc.fFovy = XMConvertToRadians(60.f);
	FreeDesc.fNear = 0.1f;
	FreeDesc.fFar = 1000.f;
	FreeDesc.fMouseSensor = 0.05f;
	FreeDesc.fSpeedPerSec = 10.f;
	FreeDesc.fRotationPerSec = 180.f;


	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &FreeDesc)))
		return E_FAIL;
	return S_OK;
}

unique_ptr<CLevel_Shelter> CLevel_Shelter::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Shelter>(new CLevel_Shelter(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Shelter"), nullptr, MB_OK);

	return pInstance;
}
