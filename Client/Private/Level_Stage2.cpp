#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"

#include "Level_Stage2.h"
#include "Inventory.h"
#include "Player.h"
#include "Zombie.h"
#include "Door.h"
#include "Blocker.h"
#include "Stair_Collider.h"
#include "Body_Zombie.h"
#include "Search_Collider.h"
#include "SearchBox.h"
#include "BoxCollider.h"

#include "Layer.h"
namespace fs = std::filesystem;
using json = nlohmann::json;
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


	//if (FAILED(Ready_Layer_Stair_Collider(TEXT("Layer_Stair_Collider"))))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_Release_Collider(TEXT("Layer_Release_Collider"))))
	//	return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Zombie(TEXT("Layer_Zombie"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Search_Collider(TEXT("Layer_Search_Collider"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Search_Box(TEXT("Layer_SearchBox"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Inven(TEXT("Layer_Inventory"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Overlay(TEXT("Layer_Overlay"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_HealthUI(TEXT("Layer_PlayerUI"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_BoxCollider(TEXT("Layer_BoxCollider"))))
		return E_FAIL;
	
	if (FAILED(Load_Door_Blocker()))
		return E_FAIL;
	if (FAILED(Load_Stair_Collider()))
		return E_FAIL;



	return S_OK;
}

void CLevel_Stage2::Update(_float fTimeDelta)
{

	if (m_bChangeLevel)
	{
		if (FAILED(CGameInstance::Get().Change_Level(ETOUI(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::SHELTER))))
			return;

		m_bChangeLevel = false;
	}
}

HRESULT CLevel_Stage2::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("스테이지2 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT::DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(CGameInstance::Get().Add_Light(LightDesc)))
		return E_FAIL;
	//LightDesc.eType = LIGHT::POINT;
	//LightDesc.vPosition = _float4(-2.f, 0.3f, 0.f, 1.f);
	//LightDesc.fRange =2.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.4f, 0.4f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 0.4f, 0.4f, 1.f);
	//
	//if (FAILED(CGameInstance::Get().Add_Light(LightDesc)))
	//	return E_FAIL;
	//
	//LightDesc.eType = LIGHT::POINT;
	//LightDesc.vPosition = _float4(0, 0.3f, 0, 1.f);
	//LightDesc.fRange = 1.f;
	//LightDesc.vDiffuse = _float4(0.4f, 1.f, 0.4f, 1.f);
	//LightDesc.vAmbient = _float4(0.2f, 0.4f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(0.4f, 1.f, 0.4f, 1.f);
	//
	//if (FAILED(CGameInstance::Get().Add_Light(LightDesc)))
	//	return E_FAIL;

	LightDesc.eType = LIGHT::SPOT;
	LightDesc.vDirection = _float4(0.f, -1.f, 0.f, 0.f);
	LightDesc.vPosition = _float4(0.2f, 0.6f, 0, 1.f);
	LightDesc.fRange = 0.4f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.fAngle = cosf(XMConvertToRadians(45.f));

	if (FAILED(CGameInstance::Get().Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Stage2::Ready_Layer_Inven(const _wstring& strLayerTag)
{
	CInventory::INVENTORY_DESC pDesc;

	pDesc.nextLevel = LEVEL::STAGE2;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_Inventory"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Stage2::Ready_Layer_HealthUI(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = 100.f;
	pDesc.fSizeY = 100.f;
	pDesc.fX = 100.f;
	pDesc.fY = g_iWinSizeY - 80.f;
	pDesc.pGameObjectTag = L"JoeIcon";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_JoeIcon"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pGameObjectTag = L"HealthBarFrame";
	pDesc.fSizeX = 300.f;
	pDesc.fSizeY = 20.f;
	pDesc.fX = 310.f;
	pDesc.fY = g_iWinSizeY - 40.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_HealthBarFrame", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pGameObjectTag = L"HealthBarFill";
	pDesc.fSizeX = 280.f;
	pDesc.fSizeY = 10.f;
	pDesc.fX = 309.f;
	pDesc.fY = g_iWinSizeY - 40.f;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_HealthBarFill", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Search_Collider(const _wstring& strLayerTag)
{
	CSearch_Collider::SEARCH_COLLIDER_DESC pDesc;
	pDesc.position = XMVectorSet(1.1f, 0.12f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.position = XMVectorSet(3.3f, 0.12f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.position = XMVectorSet(3.44f, 0.42f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.position = XMVectorSet(2.1f, 0.42f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.position = XMVectorSet(-0.77f, 0.42f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.position = XMVectorSet(-0.42f, 0.72f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.position = XMVectorSet(0.74f, 0.72f, -0.1f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	//pDesc.position = XMVectorSet(0.15f, 0.412f, -0.1f, 1);
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE1), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE1), strLayerTag, &pDesc)))
	//	return E_FAIL;
	//pDesc.position = XMVectorSet(0.52f, 0.712f, -0.1f, 1);
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE1), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE1), strLayerTag, &pDesc)))
	//	return E_FAIL;
	//pDesc.position = XMVectorSet(0.84f, 0.712f, -0.1f, 1);
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE1), L"Prototype_Search_Collider", ETOUI(LEVEL::STAGE1), strLayerTag, &pDesc)))
	//	return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Stage2::Ready_BoxCollider(const _wstring& strLayerTag)
{
	CBoxCollider::BOX_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("BoxCollider");
	pDesc.purpose = CBoxCollider::BOX::MOVE;
	pDesc.position = XMVectorSet(-5.f, 0.12f, -0.1f, 1);


	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_BoxCollider"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Stage2::Ready_Layer_Search_Box(const _wstring& strLayerTag)
{
	CSearchBox::SEARCH_DESC pSDesc;

	pSDesc.nextLevel = LEVEL::STAGE2;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_SearchBox"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pSDesc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Stage2::Ready_Layer_Overlay(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX;
	pDesc.fSizeY = g_iWinSizeY;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.5f;
	pDesc.pGameObjectTag = L"Overlay";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_Overlay"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Stage2::Load_Stair_Collider()
{
	string LevelName = "STAGE2";


	string path = "../../Resources/Data/" + LevelName + "_Stair_Collider.json";
	ifstream file(path);
	if (!file.is_open()) {
		return E_FAIL;
	}

	json j;
	file >> j;

	for (auto& gameObject : j["GameObjects"])
	{
		_wstring layerTag;
		CStair_Collider::STAIR_DESC desc;
		layerTag = StringToWString(gameObject.value("Layer", ""));
		uint32_t uState = gameObject["UpOrDown"].get<uint32_t>();
		if (uState == ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_UP)) {
			desc.state = CStair_Collider::STAIR_COLLIDER::STAIR_UP;
		}
		else if (uState == ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_DOWN)) {
			desc.state = CStair_Collider::STAIR_COLLIDER::STAIR_DOWN;
		}
		_float fRight[4], fUp[4], fLook[4], fPos[4];
		for (int i = 0; i < 4; ++i) {
			fRight[i] = gameObject["Right"][i].get<float>();
			fUp[i] = gameObject["Up"][i].get<float>();
			fLook[i] = gameObject["Look"][i].get<float>();
			fPos[i] = gameObject["Position"][i].get<float>();
		}
		XMMATRIX matWorld = XMMatrixIdentity();

		matWorld.r[0] = XMLoadFloat4((_float4*)fRight);  // Right
		matWorld.r[1] = XMLoadFloat4((_float4*)fUp);     // Up
		matWorld.r[2] = XMLoadFloat4((_float4*)fLook);   // Look
		matWorld.r[3] = XMLoadFloat4((_float4*)fPos);    // Position
		XMStoreFloat4x4(&desc.worldMat, matWorld);
		desc.pGameObjectTag = L"Stair_Collider";
		if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Stair_Collider", ETOUI(LEVEL::STAGE2), layerTag, &desc))) {
			return E_FAIL;
		}

	}

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


	CStair_Collider::STAIR_DESC stairDesc{};

	stairDesc.state = CStair_Collider::STAIR_COLLIDER::STAIR_UP;
	stairDesc.pGameObjectTag = L"Stair_Collider";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Stair_Collider"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &stairDesc)))
		return E_FAIL;

	//CGameObject::GAMEOBJECT_DESC desc;
	//desc.pGameObjectTag = L"Release_Collider";
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Release_Collider"),
	//	ETOUI(LEVEL::STAGE2), strLayerTag, &desc)))
	//	return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Player");
	pDesc.fSpeedPerSec = 10.f;
	pDesc.fRotationPerSec = 720.f;
	pDesc.nextLevel = LEVEL::STAGE2;
	pDesc.pos = XMVectorSet(-4.5f, 0, -0, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Player"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Zombie(const _wstring& strLayerTag)
{

	CZombie::ZOMBIE_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Zombie");
	pDesc.fSpeedPerSec = 0.1f;
	pDesc.fRotationPerSec = 720.f;
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::IDLE;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::BACK;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE1;
	pDesc.pos = XMVectorSet(-1.5f, 0, 1.f, 1);
	pDesc.nextLevel = LEVEL::STAGE2;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(-2.5f, 0, 1.5f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::IDLE;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::LEFT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2;
	pDesc.pos = XMVectorSet(0.5f, 0, 0.f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(-0.5f, 0.3f, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::IDLE;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::RIGHT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;

	pDesc.pos = XMVectorSet(1.5f, 0.3f, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::WALK;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::LEFT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::WALK_FAST;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(1.3f, 0.f, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::IDLE;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::LEFT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(1.8f, 0.f, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::IDLE;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::RIGHT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::PATROL_IDLE;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(2.5f, 0, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::WALK;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::RIGHT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::WALK_FAST;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(-0.5f, 0.f, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::WALK;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::RIGHT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::PATROL_WALK;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(1.f, 0.6f, 0.f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(1.6f, 0.6f, 0.f, 1);
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::WALK;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::LEFT;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Stage2::Ready_Layer_Sky(const _wstring& strLayerTag)
{

	CGameObject::GAMEOBJECT_DESC pDesc{};
	pDesc.pGameObjectTag = L"Sky";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Sky"),
		ETOUI(LEVEL::STAGE2), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}


HRESULT CLevel_Stage2::Load_Door_Blocker()
{
	string LevelName = "STAGE2";


	string path = "../../Resources/Data/" + LevelName + "_DoorAndBlocker.json";
	ifstream file(path);
	if (!file.is_open()) {
		return E_FAIL;
	}

	json j;
	file >> j;

	for (auto& gameObject : j["GameObjects"])
	{
		_wstring prototypeTag, layerTag;
		CDoor::DOOR_DESC desc;
		prototypeTag = StringToWString(gameObject.value("PrototypeTag", ""));
		desc.pGameObjectTag = StringToWString(gameObject.value("ObjectTag", ""));
		layerTag = StringToWString(gameObject.value("Layer", ""));
		desc.leftTag = StringToWString(gameObject.value("LeftObject", ""));
		desc.rightTag = StringToWString(gameObject.value("RightObject", ""));

		_float fRight[4], fUp[4], fLook[4], fPos[4];
		for (int i = 0; i < 4; ++i) {
			fRight[i] = gameObject["Right"][i].get<float>();
			fUp[i] = gameObject["Up"][i].get<float>();
			fLook[i] = gameObject["Look"][i].get<float>();
			fPos[i] = gameObject["Position"][i].get<float>();
		}
		XMMATRIX matWorld = XMMatrixIdentity();

		matWorld.r[0] = XMLoadFloat4((_float4*)fRight);  // Right
		matWorld.r[1] = XMLoadFloat4((_float4*)fUp);     // Up
		matWorld.r[2] = XMLoadFloat4((_float4*)fLook);   // Look
		matWorld.r[3] = XMLoadFloat4((_float4*)fPos);    // Position
		XMStoreFloat4x4(&desc.worldMat, matWorld);

		if (prototypeTag == L"Prototype_GameObject_Blocker") {
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Blocker", ETOUI(LEVEL::STAGE2), layerTag, &desc))) {
				return E_FAIL;
			}
		}
		else if (prototypeTag == L"Prototype_GameObject_Door") {
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Door", ETOUI(LEVEL::STAGE2), layerTag, &desc))) {
				return E_FAIL;
			}
		}
	}

	auto blockerLayer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::STAGE2), L"Layer_Blocker");
	auto doorLayer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::STAGE2), L"Layer_Door");

	auto blockers = blockerLayer->GetObjects();
	auto doors = doorLayer->GetObjects();
	for (auto& blocker : blockers) {
		auto block = static_pointer_cast<CBlocker>(blocker);
		if (block->Get_LeftTag() != L"Empty") {
			for (auto& door : doors) {
				auto pDoor = static_pointer_cast<CDoor>(door);
				if (block->Get_LeftTag() == pDoor->Get_Tag()) {
					block->Set_LeftDoor(pDoor);
					break;
				}
			}
		}
		if (block->Get_RightTag() != L"Empty") {
			for (auto& door : doors) {
				auto pDoor = static_pointer_cast<CDoor>(door);
				if (block->Get_RightTag() == pDoor->Get_Tag()) {
					block->Set_RightDoor(pDoor);
					break;
				}
			}
		}
	}
	for (auto& door : doors) {
		auto pDoor = static_pointer_cast<CDoor>(door);
		if (pDoor->Get_LeftTag() != L"Empty") {
			for (auto& blocker : blockers) {
				auto pBlocker = static_pointer_cast<CBlocker>(blocker);
				if (pDoor->Get_LeftTag() == pBlocker->Get_Tag()) {
					pDoor->Set_LeftBlocker(pBlocker);
					break;
				}
			}
		}
		if (pDoor->Get_RightTag() != L"Empty") {
			for (auto& blocker : blockers) {
				auto pBlocker = static_pointer_cast<CBlocker>(blocker);
				if (pDoor->Get_RightTag() == pBlocker->Get_Tag()) {
					pDoor->Set_RightBlocker(pBlocker);
					break;
				}
			}
		}
	}
	return S_OK;
}



unique_ptr<CLevel_Stage2> CLevel_Stage2::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Stage2>(new CLevel_Stage2(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Stage2"), nullptr, MB_OK);

	return pInstance;
}
