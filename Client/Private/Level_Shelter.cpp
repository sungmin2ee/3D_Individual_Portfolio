#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Loading.h"

#include "Level_Shelter.h"
#include "Inventory.h"
#include "Player.h"
#include "MapPin.h"
#include "Map.h"
#include "Stair_Collider.h"
#include "Zombie.h"
#include "Body_Zombie.h"
#include "BoxCollider.h"
#include "SubmitButton.h"
#include "CloseButton.h"
#include "Layer.h"
#include "Fade.h"
//#include "Overlay.h"
//#include "Sky.h"
namespace fs = std::filesystem;
using json = nlohmann::json;
CLevel_Shelter::CLevel_Shelter(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }, m_pDevice{ pDevice }, m_pContext{ pContext }
{
}

CLevel_Shelter::~CLevel_Shelter()
{ 
	CGameInstance::Get().StopAll();

}

HRESULT CLevel_Shelter::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Inven(TEXT("Layer_Inventory"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Overlay(TEXT("Layer_Overlay"))))
		return E_FAIL;
	if (FAILED(Load_Stair_Collider()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_HealthUI(TEXT("Layer_PlayerUI"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Zombie(TEXT("Layer_Zombie"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_FixUI(TEXT("Layer_FixUI"))))
		return E_FAIL;
	if (FAILED(Ready_BoxCollider(TEXT("Layer_BoxCollider"))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Load(ETOUI(LEVEL::SHELTER)))) {
		return E_FAIL;
	}
	if (FAILED(Ready_Fade(TEXT("Layer_Fade"))))
		return E_FAIL;

	CGameInstance::Get().PlayBGM(L"ShelterBGM.wav", 0.7f);
	auto zombieLayer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::SHELTER), TEXT("Layer_Zombie"));
	if (zombieLayer == nullptr)
		return E_FAIL;
	auto zombies = zombieLayer->GetObjects();

	for (auto& zombie : zombies) {
		m_vZombies.push_back(static_pointer_cast<CZombie>(zombie));
	}
	workerCount = std::max<size_t>(1, std::thread::hardware_concurrency());

	return S_OK;
}

void CLevel_Shelter::Update(_float fTimeDelta)
{

	//if (CGameInstance::Get().Key_Down(DIK_CAPITAL)) {
	//	CGameInstance::Get().Save(ETOUI(LEVEL::SHELTER));
	//}
	if (m_bEnterScene == false) {
		fade->Set_Fade(0);
		m_bEnterScene = true;
	}

	const size_t zombieCount = m_vZombies.size();
	const size_t chunkSize = (zombieCount + workerCount - 1) / workerCount;

	for (size_t w = 0; w < workerCount; ++w)
	{
		const size_t begin = w * chunkSize;
		const size_t end = std::min(begin + chunkSize, zombieCount);

		if (begin >= end)
			break;
		CGameInstance::Get().Enqueue([this, begin, end, fTimeDelta]()
			{
				for (size_t i = begin; i < end; ++i)
				{
					m_vZombies[i]->Get_Body()->Get_StateMachine()->Update(fTimeDelta);

				}
			});
	}

	CGameInstance::Get().WaitAll();

	if (m_bChangeLevel)
	{
		switch (m_NextLevel) {
		case 4:
		{
			if (fade->Get_Finished()) {
				if (FAILED(CGameInstance::Get().Change_Level(ETOUI(LEVEL::LOADING),
					CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::STAGE1))))
					return;
				m_bChangeLevel = false;

			}
			break;
		}
		case 5: 
		{
			if (fade->Get_Finished()) {
				if (FAILED(CGameInstance::Get().Change_Level(ETOUI(LEVEL::LOADING),
					CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::STAGE2))))
					return;
				m_bChangeLevel = false;

			}
			
		}
		break;

		}
	
	}
}

HRESULT CLevel_Shelter::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("쉘터레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};
	
	LightDesc.eType = LIGHT::DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);
	
	
	if (FAILED(CGameInstance::Get().Add_Light(LightDesc)))
		return E_FAIL;
	
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
HRESULT CLevel_Shelter::Load_Stair_Collider()
{
	string LevelName = "SHELTER";


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
		if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Stair_Collider", ETOUI(LEVEL::SHELTER), layerTag, &desc))) {
			return E_FAIL;
		}

	}

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Map(const _wstring& strLayerTag)
{



	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX * 0.9f;
	pDesc.fSizeY = g_iWinSizeY;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 1.5f;
	pDesc.pGameObjectTag = L"Map";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_Map", ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;


	CMapPin::MAPPIN_DESC mDesc;

	mDesc.fSizeX = g_iWinSizeX * 0.06f;
	mDesc.fSizeY = g_iWinSizeY * 0.09f;
	mDesc.fX = g_iWinSizeX * 0.5f;
	mDesc.fY = g_iWinSizeY * 0.4f;
	mDesc.pGameObjectTag = L"MapPin1";
	mDesc.textureTag = L"Prototype_Component_Texture_MapInfo1";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_MapPin", ETOUI(LEVEL::SHELTER), TEXT("Layer_Map"), &mDesc)))
		return E_FAIL;

	mDesc.fX = g_iWinSizeX * 0.7f;
	mDesc.fY = g_iWinSizeY * 0.6f;
	mDesc.pGameObjectTag = L"MapPin2";
	mDesc.textureTag = L"Prototype_Component_Texture_MapInfo2";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_MapPin", ETOUI(LEVEL::SHELTER), TEXT("Layer_Map"), &mDesc)))
		return E_FAIL;


	CUIObject::UIOBJECT_DESC buttonDesc;
	buttonDesc.fSizeX = g_iWinSizeX * 0.1f;
	buttonDesc.fSizeY = g_iWinSizeY * 0.1f;
	buttonDesc.fX = g_iWinSizeX * 0.5f;
	buttonDesc.fY = g_iWinSizeY * 0.9f;
	buttonDesc.pGameObjectTag = L"MoveButton";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_MoveButton", ETOUI(LEVEL::SHELTER), strLayerTag, &buttonDesc)))
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
	pDesc.pos = XMVectorSet(1.f, 0.01f, 0, 1);

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
HRESULT CLevel_Shelter::Ready_Layer_Zombie(const _wstring& strLayerTag)
{

	CZombie::ZOMBIE_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("Zombie");
	pDesc.fSpeedPerSec = 0.1f;
	pDesc.fRotationPerSec = 720.f;
	pDesc.firstState = CBody_Zombie::ZOMBIE_FIRSTSTATE::IDLE;
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::RIGHT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::DOORHIT;
	pDesc.pos = XMVectorSet(-0.71f, 0, -0.1f, 1);
	pDesc.nextLevel = LEVEL::SHELTER;

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.pos = XMVectorSet(-0.71f, 0.f, 0.f, 1);
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;

	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::LEFT;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2;
	pDesc.pos = XMVectorSet(-1.4f, 0.f, 0.f, 1);

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	//pDesc.pos = XMVectorSet(-1.5f, 0.f, 1.f, 1);
	//pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::RIGHT;
	//pDesc.State = CBody_Zombie::ZOMBIE_STATE::CRAWL_IDLE;
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Zombie"),
	//	ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
	//	return E_FAIL;
	pDesc.pos = XMVectorSet(-1.3f, 0.f, -0.3f, 1);
	pDesc.Direction = CBody_Zombie::ZOMBIE_DIR::BACK;
	pDesc.State = CBody_Zombie::ZOMBIE_STATE::AGGRO_IDLE2;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Zombie"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}


HRESULT CLevel_Shelter::Ready_Layer_FixUI(const _wstring& strLayerTag)
{



	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX * 0.2f;
	pDesc.fSizeY = g_iWinSizeY * 0.5f;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.5f;
	pDesc.pGameObjectTag = L"FixUI";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_FixUI", ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;


	CSubmitButton::SUBMIT_DESC buttonDesc;
	buttonDesc.fSizeX = g_iWinSizeX * 0.1f;
	buttonDesc.fSizeY = g_iWinSizeY * 0.08f;
	buttonDesc.fX = g_iWinSizeX * 0.5f;
	buttonDesc.fY = g_iWinSizeY * 0.68f;
	buttonDesc.pGameObjectTag = L"SubmitButton";
	buttonDesc.nextLevel = LEVEL::SHELTER;

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_SubmitButton", ETOUI(LEVEL::SHELTER), strLayerTag, &buttonDesc)))
		return E_FAIL;

	CCloseButton::CLOSEBUTTON_DESC cbDesc;

	cbDesc.fSizeX = g_iWinSizeX * 0.02f;
	cbDesc.fSizeY = g_iWinSizeY * 0.03f;
	cbDesc.fX = g_iWinSizeX * 0.58f;
	cbDesc.fY = g_iWinSizeY * 0.28f;
	cbDesc.pGameObjectTag = L"CloseButton";
	cbDesc.nextLevel = LEVEL::SHELTER;
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_CloseButton", ETOUI(LEVEL::SHELTER), strLayerTag, &cbDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Shelter::Ready_BoxCollider(const _wstring& strLayerTag)
{
	CBoxCollider::BOX_DESC pDesc;
	pDesc.pGameObjectTag = TEXT("BoxCollider");
	pDesc.purpose = CBoxCollider::BOX::FIX;
	pDesc.position = XMVectorSet(-0.6f, 0.12f, -0.1f, 1);


	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_BoxCollider"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;

	pDesc.pGameObjectTag = TEXT("BoxCollider");
	pDesc.purpose = CBoxCollider::BOX::MAP;
	pDesc.position = XMVectorSet(1.94f, 0.412f, -0.1f, 1);

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_BoxCollider"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Shelter::Ready_Fade(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX;
	pDesc.fSizeY = g_iWinSizeY;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.5f;
	pDesc.pGameObjectTag = L"Fade";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Fade"),
		ETOUI(LEVEL::SHELTER), strLayerTag, &pDesc)))
		return E_FAIL;


	auto fadeLayer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::SHELTER), L"Layer_Fade");
	if (fadeLayer == nullptr) return E_FAIL;
	fade = static_pointer_cast<CFade>(fadeLayer->GetObjectFirst());
	return S_OK;
}
unique_ptr<CLevel_Shelter> CLevel_Shelter::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Shelter>(new CLevel_Shelter(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Shelter"), nullptr, MB_OK);

	return pInstance;
}
