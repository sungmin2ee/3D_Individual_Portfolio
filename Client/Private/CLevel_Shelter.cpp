#include "GameInstance.h"


#include "Level_Shelter.h"
#include "BackGround.h"

CLevel_Shelter::CLevel_Shelter(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }
{
}

CLevel_Shelter::~CLevel_Shelter()
{
}

HRESULT CLevel_Shelter::Initialize()
{
	if (FAILED(Ready_Layer_Logo(TEXT("Layer_Logo"))))
		return E_FAIL;
	//if(FAILED(CGameInstance::Get().Add_Prototype()))

	return S_OK;
}

void CLevel_Shelter::Update(_float fTimeDelta)
{

	if (CGameInstance::Get().Get_DIKeyState(DIK_CAPITAL)) {
		CGameInstance::Get().Save(ETOUI(LEVEL::LOGO));
	}
	//if (GetKeyState(VK_RETURN) & 0x8000)
	//{
	//	if (FAILED(CGameInstance::Get().Change_Level(ETOUI(LEVEL::LOADING),
	//		CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::GAMEPLAY))))
	//		return;

	//	return;
	//}
}

HRESULT CLevel_Shelter::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Logo(const _wstring& strLayerTag)
{
	CBackGround::BACKGROUND_DESC		Desc{};
	Desc.iData = 10;

	CGameInstance::Get().Load(ETOUI(LEVEL::LOGO));

	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
	//	ETOUI(LEVEL::LOGO), strLayerTag, &Desc)))
	//	return E_FAIL;

	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_Test"),
	//	ETOUI(LEVEL::LOGO), strLayerTag, &Desc)))
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
