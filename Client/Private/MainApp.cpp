#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ImguiHandler.h"
#include "Helper.h"
#include "VIBuffer_Collider.h"
#include "Obb.h"
#include "Shader.h"
#include "CModelObject.h"
#include "Model.h"
#include "Camera_Free.h"
#include "StateMachine.h"
#include "Stair_Collider.h"
#include "VIBuffer_Cube.h"
#include "Door.h"
#include "Blocker.h"
#include "Body_Player.h"
#include "Body_Zombie.h"
#include "Weapon.h"
#include "Blood.h"
#include "MainPic.h"


CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
//	m_pImguiHandler.reset();
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
	if (CGameInstance::Get().Key_Up(DIK_HOME)) {
		SaveLoadEnable = !SaveLoadEnable;
	}
	if (SaveLoadEnable) {
		//if (CGameInstance::Get().Key_Up(DIK_9)) {
		//	m_pImguiHandler->Save_DoorAndBlocker();
		//}
		//if (CGameInstance::Get().Key_Up(DIK_0)) {
		//	m_pImguiHandler->Load_DoorAndBlocker();
		//}
		//if (CGameInstance::Get().Key_Up(DIK_K)) {
		//	m_pImguiHandler->Save_StairCollider();
		//}
		//if (CGameInstance::Get().Key_Up(DIK_L)) {
		//	m_pImguiHandler->Load_StairCollider();
		//}
	}
	//D3D11_SAMPLER_DESC
	//D3D11_DEPTH_STENCIL_DESC desc;

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

	/* Prototype_Component_VIBuffer_Rect */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	/*Inventory Textures*/
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/inventory.dds"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Blocker"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/black.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_smaller_item_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/smaller_item_border.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_equip_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/equip_border.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_search_border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/clear_border.png"), 1))))
		return E_FAIL;
	//Icons

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_lockpick"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_lockpick.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_bandage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_bandage.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_metal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_metal.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_lumber"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_lumber_0.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_meat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_meat.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_connectors"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_connectors.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_healing_kit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_healing_kit.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_tape"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_tape.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_search"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_search.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_dust_scratch_overlay"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/dust_scratch_overlay.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_Map"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_Map.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_Fix"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_Fix.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_Exit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_Exit.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_door_fix_ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/door_fix_ui.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_SubmitButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/SubmitButton.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_closeButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/closeButton.png"), 1))))
		return E_FAIL;

	/////

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNonAnim"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl"), VERTEX::Elements, VERTEX::iNumElements))))
		return E_FAIL;
	//charactor icon
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_joe"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_jo.png"), 1))))
		return E_FAIL;

	//Health Icon
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_Health_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/icon_BarricadeFrame4.png"), 1))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_Health_Fill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/healthbar_fill.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_HpBar */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_HpBar"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_HpBar.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Collider"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Collider.hlsl"), VCOLLIDER::Elements, VCOLLIDER::iNumElements))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Blocker"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Blocker.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNoLightMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNoLightMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Collider_Buffer"),
		VIBuffer_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_OBB"),
		Obb::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//model
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_ModelObject"),
		CModelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Door"),
		CModel::Create(m_pDevice, m_pContext, ETOUI(MODEL::NONANIM), "../../Resources/Models/Door.fbx"))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Blocker"),
		CModel::Create(m_pDevice, m_pContext, ETOUI(MODEL::NONANIM), "../../Resources/Models/Blocker.fbx"))))
		return E_FAIL;

	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Player */
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Joe"),
		CModel::Create(m_pDevice, m_pContext, ETOUI(MODEL::ANIM), "../../Resources/Models/Player1.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Zombie1"),
		CModel::Create(m_pDevice, m_pContext, ETOUI(MODEL::ANIM), "../../Resources/Models/Zombie1.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Model_Axe"),
		CModel::Create(m_pDevice, m_pContext, ETOUI(MODEL::NONANIM), "../../Resources/Models/FireAxe.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Body_Zombie"),
		CBody_Zombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//sky

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Sky_%d.dds"), 2))))
		return E_FAIL;

	//camera

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Collider
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Stair_Collider"),
		CStair_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//DOOR AND BLOCKER

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Door"),
		CDoor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Blocker"),
		CBlocker::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//Axe

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_TitleBG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Logo_Bg_1.png"), 1))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_TitleBG"),
		CMainPic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*CVIBuffer_Particle_Point::PARTICLE_INSTANCE_DESC		ExploDesc{};
	ExploDesc.iNumInstances = 5;
	ExploDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ExploDesc.vRange = _float3(1.f, 0.2f, 1.f);
	ExploDesc.vSize = _float2(0.05f, 0.1f);
	ExploDesc.vSpeed = _float2(0.2f, 1.f);
	ExploDesc.vLifeTime = _float2(0.3f, 0.7f);
	ExploDesc.isLoop = true;
	ExploDesc.vPivot = ExploDesc.vCenter;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Particle_Blood"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, &ExploDesc))))
		return E_FAIL;*/


	CVIBuffer_Particle_Point::PARTICLE_INSTANCE_DESC		bloodDesc{};
	bloodDesc.iNumInstances = 100;
	bloodDesc.vCenter = _float3(0.f, 2.43f, 0.f);
	bloodDesc.vRange = _float3(0.f, 0.f, 0.f);
	bloodDesc.vSize = _float2(0.01f, 0.03f);
	bloodDesc.vSpeed = _float2(1.f, 3.f);
	bloodDesc.vLifeTime = _float2(0.3f, 1.2f);
	bloodDesc.isLoop = false;
	bloodDesc.vPivot = bloodDesc.vCenter;


	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Particle_Blood"),
		CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext, &bloodDesc))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Blood"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/blood_drip_splash_a_bsc.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance_Particle_Point */
	if (FAILED(CGameInstance::Get().Add_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxInstance_Particle_Point"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_Particle_Point.hlsl"), VTXINSTANCE_PARTICLE_POINT::Elements, VTXINSTANCE_PARTICLE_POINT::iNumElements))))
		return E_FAIL;





	return S_OK;
}






unique_ptr<CMainApp> Client::CMainApp::Create() {

	auto pInstance = unique_ptr<CMainApp>(new CMainApp());

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to create MainApp");
	return pInstance;
}



