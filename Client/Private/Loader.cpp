#include "Loader.h"

#include "GameInstance.h"
#include "BackGround.h"
#include "CModel.h"
#include "Shader.h"
#include "Inventory.h"

#include "ItemFrame.h"
#include "ItemIcon.h"
#include "EquipBorder.h"
#include "Player.h"
#include "Zombie.h"
#include "Body_Player.h"
#include "Body_Zombie.h"
#include "Sky.h"
#include "JoeIcon.h"
#include "HealthBarFrame.h"
#include "HealthBarFill.h"
#include "SearchBox.h"
#include "Search_Collider.h"
#include "SearchItemFrame.h"
#include "SearchItemIcon.h"
#include "Overlay.h"
#include "Blood.h"
#include "Map.h"
#include "MapPin.h"
#include "MapInfo.h"
#include "MoveButton.h"
#include "MainPic.h"
#include "GameStartButton.h"
#include "Title.h"
#include "GameEnd.h"
#include "SubmitButton.h"
#include "CloseButton.h"
#include "BoxCollider.h"
#include "FixUI.h"
#include "QuestUI.h"
#include "Nuclear.h"
#include "WhiteRect.h"
#include "Bomb.h"


CLoader::CLoader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
}

CLoader::~CLoader()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);
}

uint32_t APIENTRY ThreadMain(void* pArg)
{
	auto		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelIndex)
{
	InitializeCriticalSection(&m_CriticalSection);

	m_eNextLevelIndex = eNextLevelIndex;
	Load_Models_From_Directory(LEVEL::STATIC, "Static");

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	HRESULT		hr = {};

	switch (m_eNextLevelIndex)
	{
	case LEVEL::LOADING:
		hr = Loading_For_Loading();
		break;
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL::SHELTER:
		hr = Loading_For_Shelter();
		break;
	case LEVEL::STAGE1:
		hr = Loading_For_Stage1();
		break;
	case LEVEL::STAGE2:
		hr = Loading_For_Stage2();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	CoUninitialize();

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

#ifdef _DEBUG
void CLoader::Output_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

#endif

HRESULT CLoader::Loading_For_Loading()
{


	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	//if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_Component_Texture_TitleBG"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Logo_Bg_1.png"), 1))))
	//	return E_FAIL;
	//m_iLoadedResources++;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_Component_Texture_GameStart"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/GameStart.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Title"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Logo_OurDarkestDays_01.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_Component_Texture_GameEnd"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/GameEnd.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));
	//Load_Models_From_Directory(LEVEL::LOGO, "Logo");

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	
	

	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));
	

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_GameStart"),
		CGameStartButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_Title"),
		CTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_GameEnd"),
		CGameEnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Shelter()
{

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	//Map

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_Map"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/map_35.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_MapPin"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/MapPin.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_MapInfo1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/MapSchool.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_MapInfo2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/MapOffice.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_MoveButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/MoveScene.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_QuestUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/location_overwhelmed.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_Nuclear"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/nuclear.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_White"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/white.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Model_Bomb"),
		CModel::Create(m_pDevice, m_pContext, ETOUI(MODEL::NONANIM), "../../Resources/Models/Missile.fbx"))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), L"Prototype_Overlay", COverlay::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), L"Prototype_Inventory", CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), L"Prototype_ItemFrame", CItemFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), L"Prototype_ItemIcon", CItemIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), L"Prototype_EquipBorder", CEquipBorder::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_JoeIcon"),
		CJoeIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_HealthBarFill"),
		CHealthBarFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_HealthBarFrame"),
		CHealthBarFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Map"),
		CMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_MapPin"),
		CMapPin::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_MapInfo"),
		CMapInfo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_MoveButton"),
		CMoveButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Blood"),
		CBlood::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Zombie"),
		CZombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_FixUI"),
		CFixUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_SubmitButton"),
		CSubmitButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_CloseButton"),
		CCloseButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_BoxCollider"),
		CBoxCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_QuestUI"),
		CQuestUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Nuclear"),
		CNuclear::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_WhiteRect"),
		CWhiteRect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_GameObject_Bomb"),
		CBomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage1()
{

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));


	
	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));

	/* For.Prototype_GameObject_Blood */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_GameObject_Blood"),
		CBlood::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_Overlay", COverlay::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_Search_Collider", CSearch_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_SearchItemFrame", CSearchItemFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_SearchItemIcon", CSearchItemIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_SearchBox", CSearchBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_Inventory", CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_ItemFrame", CItemFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_ItemIcon", CItemIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), L"Prototype_EquipBorder", CEquipBorder::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_GameObject_Zombie"),
		CZombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_JoeIcon"),
		CJoeIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_HealthBarFill"),
		CHealthBarFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_HealthBarFrame"),
		CHealthBarFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE1), TEXT("Prototype_GameObject_BoxCollider"),
		CBoxCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage2()
{

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));



	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//Load_Models_From_Directory(LEVEL::GAMEPLAY, "Gameplay");

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));

	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_Overlay", COverlay::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_Search_Collider", CSearch_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_SearchItemFrame", CSearchItemFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_SearchItemIcon", CSearchItemIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_SearchBox", CSearchBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_Inventory", CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_ItemFrame", CItemFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_ItemIcon", CItemIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), L"Prototype_EquipBorder", CEquipBorder::Create(m_pDevice, m_pContext))))
		return E_FAIL;



;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Zombie"),
		CZombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_JoeIcon"),
		CJoeIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_HealthBarFill"),
		CHealthBarFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_HealthBarFrame"),
		CHealthBarFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sky */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_Blood"),
		CBlood::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_GameObject_BoxCollider"),
		CBoxCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

unique_ptr<CLoader> CLoader::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, LEVEL eNextLevelIndex)
{
	auto	pInstance = unique_ptr<CLoader>(new CLoader(pDevice, pContext));

	if (FAILED(pInstance->Initialize(eNextLevelIndex)))
		MSG_BOX("Failed to Created : CLoader");

	return pInstance;
}
namespace fs = std::filesystem;
using json = nlohmann::json;

void CLoader::Load_Models_From_Directory(LEVEL eNextLevelIndex, const string& strSceneName) {
    string strPath = "../../Resources/Models/";
	json sceneJson;

    // 2. 해당 폴더 내부를 순회
    for (const auto& entry : fs::directory_iterator(strPath)) {
        if (entry.path().extension() == ".fbx") { // FBX 파일만 골라내기
            string strFileName = entry.path().stem().string(); // 확장자 뺀 파일명 (예: "Player")

			json modelData;
			modelData["FilePath"] = strPath + strFileName + ".fbx";
			modelData["PrototypeTag"] = "Prototype_Model_" + strFileName;

			sceneJson["Model"].push_back(modelData);
        }
	
    }
	string savePath = "../../Resources/Data/Model_List.json";
	std::ofstream file(savePath);
	if (file.is_open()) {
		file << sceneJson.dump(4); // 4는 들여쓰기(Tab) 간격입니다.
		file.close();
	}




	string strPath1 = "../../Client/Bin/ShaderFiles/";
	json shaderJson;

	// 2. 해당 폴더 내부를 순회
	for (const auto& entry : fs::directory_iterator(strPath1)) {
		if (entry.path().extension() == ".hlsl") { // hlsl 파일만 골라내기
			string strShaderName = entry.path().stem().string(); // 확장자 뺀 파일명 (예: "Player")

			json shaderData;
			shaderData["FilePath"] = strPath1 + strShaderName;
			shaderData["PrototypeTag"] = "Prototype_Component_" + strShaderName;

			shaderJson["Shader"].push_back(shaderData);
		}

	}
	string savePath1 = "../../Resources/Data/Shader_List.json";
	std::ofstream file1(savePath1);
	if (file1.is_open()) {
		file1 << shaderJson.dump(4); // 4는 들여쓰기(Tab) 간격입니다.
		file1.close();
	}



}