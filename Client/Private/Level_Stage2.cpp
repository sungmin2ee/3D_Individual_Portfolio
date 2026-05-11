#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"

#include "Level_Stage2.h"
#include "Inventory.h"
#include "Player.h"

CLevel_Stage2::CLevel_Stage2(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }, m_pDevice{ pDevice }, m_pContext{ pContext }
{
}

CLevel_Stage2::~CLevel_Stage2()
{
}

HRESULT CLevel_Stage2::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Load(ETOUI(LEVEL::STAGE2)))) {
		return E_FAIL;
	}



	return S_OK;
}

void CLevel_Stage2::Update(_float fTimeDelta)
{

	//if (CGameInstance::Get().Key_Down(DIK_CAPITAL)) {
	//	CGameInstance::Get().Save(ETOUI(LEVEL::STAGE2));
	//}

}

HRESULT CLevel_Stage2::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("스테이지2 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_UI(const _wstring& strLayerTag)
{
	//CInventory::INVENTORY_DESC pDesc;
	//
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Inventory"),
	//	ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
	//	return E_FAIL;


	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_Inventory", ETOUI(LEVEL::SHELTER), strLayerTag)))
	//	return E_FAIL;

		/* For.Prototype_GameObject_Player*/
	CPlayer::PLAYER_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Player");
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = 180.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Player"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Camera(const _wstring& strLayerTag)
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
		return E_FAIL;
	return S_OK;
}

unique_ptr<CLevel_Stage2> CLevel_Stage2::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Stage2>(new CLevel_Stage2(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Stage2"), nullptr, MB_OK);

	return pInstance;
}
