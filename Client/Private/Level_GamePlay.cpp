#include "Level_GamePlay.h"

CLevel_GamePlay::CLevel_GamePlay(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }
{
}

CLevel_GamePlay::~CLevel_GamePlay()
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	uint32_t iData = 10;
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
#endif

	return S_OK;
}

unique_ptr<CLevel_GamePlay> CLevel_GamePlay::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CLevel_GamePlay>(new CLevel_GamePlay(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
		MessageBox(g_hWnd, TEXT("Failed to Created : CLevel_GamePlay"), nullptr, MB_OK);

	return pInstance;
}
