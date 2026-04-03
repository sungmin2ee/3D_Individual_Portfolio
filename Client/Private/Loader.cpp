#include "Loader.h"

#include "GameInstance.h"
#include "BackGround.h"

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
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}
	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩 중 입니다."));
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}
	lstrcpy(m_szLoadingText, TEXT("객체원형 생성 중 입니다."));

	/* Prototype_GameObject_BackGround */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
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
	for (size_t i = 0; i < 99999999; i++)
	{
		int a = 10;
	}
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
