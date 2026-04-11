#include "Loader.h"

#include "GameInstance.h"
#include "BackGround.h"
#include "Player.h"
#include "Model.h"
#include "Shader.h"

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
	Load_Models_From_Directory(LEVEL::LOGO, "Static");
	//if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO),
	//	TEXT("Prototype_Player_Model"), CGameInstance::Get().Load("../../Resources/Models/Joe5.fbx"))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO),
	//	TEXT("Prototype_Player_Model"), CGameInstance::Get().Load("../../Resources/Models/Joe5.fbx"))))
	//{
	//	return E_FAIL;
	//}

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
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
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

HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));

	//if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO),
	//	TEXT("Prototype_Player_Model"), CGameInstance::Get().Load("../../Resources/Models/Joe5.fbx"))))
	//{
	//	return E_FAIL;
	//}

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_NonAnimShander"),
		Shader::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}
	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));
	
	/* Prototype_GameObject_BackGround */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
 	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩 중 입니다."));



	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}
	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}
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
    // 1. 경로 설정 (예: "../../Resources/Scene_Logo/")
    string strPath = "../../Resources/Models/" + strSceneName + "/Anim/";
	json sceneJson;

	string strLevelKey = "Scene_" + strSceneName;
    // 2. 해당 폴더 내부를 순회
    for (const auto& entry : fs::directory_iterator(strPath)) {
        if (entry.path().extension() == ".fbx") { // FBX 파일만 골라내기
            string strFileName = entry.path().stem().string(); // 확장자 뺀 파일명 (예: "Player")
            string strFullPath = entry.path().string();        // 전체 경로
			_wstring wFileName = StringToWString(strFileName);
            // 3. 모델 원본(Prototype) 생성
            auto pModelProto = Model::Create(m_pDevice, m_pContext, strFullPath);
            wstring wstrProtoTag = L"Prototype_Model_" + wFileName;
            CGameInstance::Get().Add_Prototype(ETOUI(eNextLevelIndex),wstrProtoTag, unique_ptr<CPrototype>(std::move(pModelProto)));

			json modelData;
			modelData["Model"] = "Prototype_Model_" + strFileName;
			modelData["IsAnim"] = true;           

			sceneJson[strLevelKey]["Model"].push_back(modelData);
        }
	
    }


	string strPath1 = "../../Resources/Models/" + strSceneName + "/NonAnim/";

	// 2. 해당 폴더 내부를 순회
	for (const auto& entry : fs::directory_iterator(strPath1)) {
		if (entry.path().extension() == ".fbx") { // FBX 파일만 골라내기
			string strFileName = entry.path().stem().string(); // 확장자 뺀 파일명 (예: "Player")
			string strFullPath = entry.path().string();        // 전체 경로
			_wstring wFileName = StringToWString(strFileName);
			// 3. 모델 원본(Prototype) 생성
			auto pModelProto = Model::Create(m_pDevice, m_pContext, strFullPath);
			wstring wstrProtoTag = L"Prototype_Model_" + wFileName;
			CGameInstance::Get().Add_Prototype(ETOUI(eNextLevelIndex), wstrProtoTag, unique_ptr<CPrototype>(std::move(pModelProto)));

			json modelData;
			modelData["Model"] = "Prototype_Model_" + strFileName;
			modelData["IsAnim"] = false;           // 일단 기본값

			sceneJson[strLevelKey]["Model"].push_back(modelData);
		}

	}
	string savePath = "../../Resources/Data/" + strSceneName + "_List.json";
	std::ofstream file(savePath);
	if (file.is_open()) {
		file << sceneJson.dump(4); // 4는 들여쓰기(Tab) 간격입니다.
		file.close();
	}
}