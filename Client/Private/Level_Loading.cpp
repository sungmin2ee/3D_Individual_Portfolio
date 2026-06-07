#include "Level_Loading.h"
#include "Loader.h"

#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Shelter.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"
#include "UIObject.h"
//#include "Level_Stage2.h"


CLevel_Loading::CLevel_Loading(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CLevel{ pDevice, pContext }
{
}

CLevel_Loading::~CLevel_Loading()
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelIndex)
{
	m_eNextLevelIndex = eNextLevelIndex;

	/* 로딩에 필요한 객체를 생성한다.  */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/* 다음레벨에 필요한 자원을 로드해주기위한 준비작업(로딩작업을 수행해줄 스레드를 생성한다)을 수행한다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelIndex);
	if (nullptr == m_pLoader)
		return E_FAIL;


	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished() &&
		GetKeyState(VK_SPACE) & 0x8000)
	{

		unique_ptr<CLevel>		pNewLevel = { nullptr };

		switch (m_eNextLevelIndex)
		{
		case LEVEL::LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL::SHELTER:
			pNewLevel = CLevel_Shelter::Create(m_pDevice, m_pContext);
			break;
		case LEVEL::STAGE1:
			pNewLevel = CLevel_Stage1::Create(m_pDevice, m_pContext);
			break;
		case LEVEL::STAGE2:
			pNewLevel = CLevel_Stage2::Create(m_pDevice, m_pContext);
			break;
		}


		if (FAILED(CGameInstance::Get().Change_Level(ETOUI(m_eNextLevelIndex), std::move(pNewLevel))))
			return;
		return;
	}



}

HRESULT CLevel_Loading::Render()
{
#ifdef _DEBUG
	m_pLoader->Output_LoadingText();
#endif
	//CGameInstance::Get().RenderText(1, L"그들은 소리에 민감합니다.", g_iWinSizeX * 0.3f, g_iWinSizeY * 0.5f, DirectX::Colors::Gold, 1.5f);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	CUIObject::UIOBJECT_DESC pDesc;
	pDesc.fSizeX = g_iWinSizeX;
	pDesc.fSizeY = g_iWinSizeY;
	pDesc.fX = g_iWinSizeX * 0.5f;
	pDesc.fY = g_iWinSizeY * 0.5f;
	pDesc.pGameObjectTag = L"TitleBG";
	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_TitleBG"),
		ETOUI(LEVEL::LOADING), strLayerTag, &pDesc)))
		return E_FAIL;
	return S_OK;
}

unique_ptr<CLevel_Loading> CLevel_Loading::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext, LEVEL eNextLevelIndex)
{
	auto	pInstance = unique_ptr<CLevel_Loading>(new CLevel_Loading(pDevice, pContext));

	if (FAILED(pInstance->Initialize(eNextLevelIndex)))
		MSG_BOX("Failed to Created : CLevel_Loading");

	return pInstance;
}
