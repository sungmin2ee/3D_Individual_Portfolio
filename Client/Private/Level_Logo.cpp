#include "Level_Logo.h"
#include "GameInstance.h"

#include "Camera_Free.h"

#include "Level_Loading.h"
#include "BackGround.h"
#include "UIObject.h"

CLevel_Logo::CLevel_Logo(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }
{
}

CLevel_Logo::~CLevel_Logo()
{
	CGameInstance::Get().StopAll();
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_Logo(TEXT("Layer_Logo"))))
		return E_FAIL;
	//if(FAILED(CGameInstance::Get().Add_Prototype()))
	CGameInstance::Get().PlayBGM(L"LogoBGM.wav", 0.7f);
	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{

	/*if (CGameInstance::Get().Get_DIKeyState(DIK_CAPITAL)) {
		CGameInstance::Get().Save(ETOUI(LEVEL::LOGO));
	}*/

	if (m_bChangeLevel)
	{
		if (FAILED(CGameInstance::Get().Change_Level(ETOUI(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::SHELTER
			))))
			return;

		m_bChangeLevel = false;
	}
}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Lights()
{
	//LIGHT_DESC			LightDesc{};
	//
	//LightDesc.eType = LIGHT::DIRECTIONAL;
	//LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	//
	//if (FAILED(CGameInstance::Get().Add_Light(LightDesc)))
	//	return E_FAIL;
	//
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Logo(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX;
	pDesc.fSizeY = g_iWinSizeY;
	pDesc.fX = g_iWinSizeX *0.5f;
	pDesc.fY = g_iWinSizeY * 0.5f;
	pDesc.pGameObjectTag = L"TitleBG";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_TitleBG"),
		ETOUI(LEVEL::LOGO), strLayerTag, &pDesc)))
		return E_FAIL;

	pDesc.fSizeX = g_iWinSizeX * 0.1f;
	pDesc.fSizeY = g_iWinSizeY * 0.1f;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.7f;
	pDesc.pGameObjectTag = L"StartButton";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_GameStart"),
		ETOUI(LEVEL::LOGO), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.fSizeX = g_iWinSizeX * 0.1f;
	pDesc.fSizeY = g_iWinSizeY * 0.1f;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.9f;
	pDesc.pGameObjectTag = L"EndButton";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_GameEnd"),
		ETOUI(LEVEL::LOGO), strLayerTag, &pDesc)))
		return E_FAIL;
	pDesc.fSizeX = g_iWinSizeX * 0.6f;
	pDesc.fSizeY = g_iWinSizeY * 0.5f;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.3f;
	pDesc.pGameObjectTag = L"Title";

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_Title"),
		ETOUI(LEVEL::LOGO), strLayerTag, &pDesc)))
		return E_FAIL;

	CGameInstance::Get().Load(ETOUI(LEVEL::LOGO));
	
	//CCamera_Free::CAMERA_FREE_DESC		FreeDesc{};
	//FreeDesc.vEye = _float4(0.f, 10.f, -5.f, 1.f);
	//FreeDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	//FreeDesc.fFovy = XMConvertToRadians(60.f);
	//FreeDesc.fNear = 0.1f;
	//FreeDesc.fFar = 1000.f;
	//FreeDesc.fMouseSensor = 0.02f;
	//FreeDesc.fSpeedPerSec = 10.f;
	//FreeDesc.fRotationPerSec = 180.f;
	//
	//
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
	//	ETOUI(LEVEL::LOGO), strLayerTag, &FreeDesc)))
	//	return E_FAIL;
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
	//	ETOUI(LEVEL::LOGO), strLayerTag, &Desc)))
	//	return E_FAIL;

	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_Test"),
	//	ETOUI(LEVEL::LOGO), strLayerTag, &Desc)))
	//	return E_FAIL;
	return S_OK;
}

unique_ptr<CLevel_Logo> CLevel_Logo::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Logo>(new CLevel_Logo(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Logo"), nullptr, MB_OK);

	return pInstance;
}
