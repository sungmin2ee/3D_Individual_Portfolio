#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"

#include "Level_Shelter.h"
#include "Inventory.h"
#include "Player.h"
//#include "Overlay.h"
//#include "Sky.h"

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
	if (FAILED(Ready_Layer_Inven(TEXT("Layer_Inventory"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Overlay(TEXT("Layer_Overlay"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_HealthUI(TEXT("Layer_PlayerUI"))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Load(ETOUI(LEVEL::SHELTER)))) {
		return E_FAIL;
	}



	return S_OK;
}

void CLevel_Shelter::Update(_float fTimeDelta)
{

	if (CGameInstance::Get().Key_Down(DIK_CAPITAL)) {
		CGameInstance::Get().Save(ETOUI(LEVEL::SHELTER));
	}
	//if (CGameInstance::Get().Key_Down(DIK_INSERT))
	//{
	//	if (FAILED(CGameInstance::Get().Change_Level(ETOUI(LEVEL::LOADING),
	//		CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::STAGE1))))
	//		return;
	//
	//	return;
	//}
}

HRESULT CLevel_Shelter::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("쉘터레벨입니다."));
#endif

	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Layer_HealthUI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = 100.f;
	pDesc.fSizeY = 100.f;
	pDesc.fX = 100.f;
	pDesc.fY = g_iWinSizeY - 80.f;
	pDesc.pGameObjectTag = L"JoeIcon";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_JoeIcon"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pGameObjectTag = L"HealthBarFrame";
	pDesc.fSizeX = 300.f;
	pDesc.fSizeY = 20.f;
	pDesc.fX = 310.f;
	pDesc.fY = g_iWinSizeY - 40.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_HealthBarFrame", ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pGameObjectTag = L"HealthBarFill";
	pDesc.fSizeX = 280.f;
	pDesc.fSizeY = 10.f;
	pDesc.fX = 309.f;
	pDesc.fY = g_iWinSizeY - 40.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_HealthBarFill", ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Layer_Inven(const _wstring& strLayerTag)
{
	CInventory::INVENTORY_DESC pDesc;

	pDesc.nextLevel = LEVEL::SHELTER;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Inventory"),
		ETOUI(LEVEL::SHELTER), strLayerTag,&pDesc)))
		return E_FAIL;


	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_Inventory", ETOUI(LEVEL::SHELTER), strLayerTag)))
	//	return E_FAIL;
	


	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Layer_Sky(const _wstring& strLayerTag)
{

	CGameObject::GAMEOBJECT_DESC pDesc{};
	pDesc.pGameObjectTag = L"Sky";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Sky"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Player");
	pDesc.fSpeedPerSec = 0.1f;
	pDesc.fRotationPerSec = 720.f;
	pDesc.nextLevel = LEVEL::SHELTER;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Player"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Layer_Overlay(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX;
	pDesc.fSizeY = g_iWinSizeY;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.5f;
	pDesc.pGameObjectTag = L"Overlay";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Overlay"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC		FreeDesc{};
	FreeDesc.vEye = _float4(0.f, 0.f, -1.f, 1.f);
	FreeDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	FreeDesc.fFovy = XMConvertToRadians(60.f);
	FreeDesc.fNear = 0.1f;
	FreeDesc.fFar = 1000.f;
	FreeDesc.fMouseSensor = 0.02f;
	FreeDesc.fSpeedPerSec = 2.f;
	FreeDesc.fRotationPerSec = 180.f;
	
	
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &FreeDesc)))
	//	return E_FAIL;
	return S_OK;
}

unique_ptr<CLevel_Shelter> CLevel_Shelter::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Shelter>(new CLevel_Shelter(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Shelter"), nullptr, MB_OK);

	return pInstance;
}
