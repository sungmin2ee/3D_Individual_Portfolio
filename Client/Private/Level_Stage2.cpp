#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"

#include "Level_Stage2.h"
#include "Inventory.h"
#include "Player.h"
#include "Zombie.h"
#include "Stair_Collider.h"

CLevel_Stage2::CLevel_Stage2(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }, m_pDevice{ pDevice }, m_pContext{ pContext }
{
}

CLevel_Stage2::~CLevel_Stage2()
{
}

HRESULT CLevel_Stage2::Initialize()
{
	if (FAILED(CGameInstance::Get().Load(ETOUI(LEVEL::STAGE2)))) {
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Stair_Collider(TEXT("Layer_Stair_Collider"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Zombie(TEXT("Layer_Zombie"))))
		return E_FAIL;



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
		ETOUI(LEVEL::STAGE2), strLayerTag, &FreeDesc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Stage2::Ready_Layer_Stair_Collider(const _wstring& strLayerTag)
{
	CGameObject::GAMEOBJECT_DESC pDesc{};
	pDesc.pGameObjectTag = L"Sky";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Sky"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	CStair_Collider::STAIR_DESC stairDesc{};

	stairDesc.state = CStair_Collider::STAIR_COLLIDER::STAIR_UP;
	stairDesc.pGameObjectTag = L"Stair_Collider";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Stair_Collider"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &stairDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Player");
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = 720.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Player"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Zombie(const _wstring& strLayerTag)
{
	CZombie::ZOMBIE_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Zombie");
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = 720.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
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
