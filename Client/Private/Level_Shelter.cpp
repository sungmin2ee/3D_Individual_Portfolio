#include "Level_Logo.h"
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
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	//if(FAILED(CGameInstance::Get().Add_Prototype()))

	return S_OK;
}

void CLevel_Shelter::Update(_float fTimeDelta)
{

	if (CGameInstance::Get().Get_DIKeyState(DIK_CAPITAL)) {
		CGameInstance::Get().Save(ETOUI(LEVEL::SHELTER));
	}

}

HRESULT CLevel_Shelter::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("쉘터레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_UI(const _wstring& strLayerTag)
{

	//CGameInstance::Get().Load(ETOUI(LEVEL::SHELTER));

	return S_OK;
}

HRESULT CLevel_Shelter::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	return S_OK;
}

unique_ptr<CLevel_Shelter> CLevel_Shelter::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_Shelter>(new CLevel_Shelter(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_Shelter"), nullptr, MB_OK);

	return pInstance;
}
