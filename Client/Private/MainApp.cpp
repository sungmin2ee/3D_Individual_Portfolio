#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ImguiHandler.h"
#include "Helper.h"
#include "VIBuffer_Cube.h"
#include "Obb.h"
#include "Shader.h"
#include "CModelObject.h"
#include "Model.h"


CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
	CGameInstance::Get().Release_Engine();
}

HRESULT CMainApp::Initialize()
{

	


	ENGINE_DESC EngineDesc{};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.hInst = g_hInstance;
	EngineDesc.eWinMode = WINMODE::WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ETOUI(LEVEL::END);

	if (FAILED(CGameInstance::Get().Initialize_Engine(EngineDesc, m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(Ready_Prototypes()))
		return E_FAIL;
	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	m_pImguiHandler = CImguiHandler::Create(EngineDesc, m_pDevice, m_pContext);
	if (nullptr == m_pImguiHandler)
		return E_FAIL;
	m_pImguiHandler->Initialize();
	return S_OK;
}

void CMainApp::Update(float fTimeDelta)
{
	CGameInstance::Get().Update_Engine(fTimeDelta);
	m_pImguiHandler->Handle_Imgui(CGameInstance::Get().GetCurLevelIndex(), fTimeDelta);
}

HRESULT CMainApp::Render()
{
	_float4			vClearColor = { 0.f, 0.f, 1.f, 1.f };

	if (FAILED(CGameInstance::Get().Clear_BackBuffer_View(&vClearColor)))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Draw()))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Present()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevelIndex)
{
	
	if (FAILED(CGameInstance::Get().Change_Level(static_cast<uint32_t>(LEVEL::LOADING),
		CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelIndex))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototypes()
{
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements))))
		return E_FAIL;
	/* Prototype_Component_VIBuffer_Rect */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*Inventory Textures*/
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Bin/Resources/Textures/Inventory.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_clear_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Bin/Resources/Textures/clear_border.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_equip_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Bin/Resources/Textures/equip_border.png"), 1))))
		return E_FAIL;
	//Icons

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_equip_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Bin/Resources/Textures/equip_border.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_equip_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Bin/Resources/Textures/equip_border.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_equip_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Bin/Resources/Textures/equip_border.png"), 1))))
		return E_FAIL;



	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNonAnim"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl"), VERTEX::Elements, VERTEX::iNumElements))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Collider"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Collider.hlsl"), VCOLLIDER::Elements, VCOLLIDER::iNumElements))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Cube_Buffer"),
		VIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_OBB"),
		Obb::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_ModelObject"),
		CModelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Joe5"), Model::Create(m_pDevice,m_pContext,"../../Resources/Models/Joe5.fbx"));

	return S_OK;
}





unique_ptr<CMainApp> Client::CMainApp::Create() {

	auto pInstance = unique_ptr<CMainApp>(new CMainApp());

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to create MainApp");
	return pInstance;
}


