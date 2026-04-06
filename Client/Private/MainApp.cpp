#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

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
	EngineDesc.eWinMode = WINMODE::WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ETOUI(LEVEL::END);



	
	if (FAILED(CGameInstance::Get().Initialize_Engine(EngineDesc, m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;
	
	return S_OK;
}

void CMainApp::Update(float fTimeDelta)
{
	CGameInstance::Get().Update_Engine(fTimeDelta);
	static bool show_demo = true; // 프로그램이 꺼질 때까지 상태가 유지됨

	if (show_demo)
	{
		ImGui::ShowDemoWindow(&show_demo);
	}
	_float fps = 1 / fTimeDelta;
	ImGui::Begin("My First Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선
	if (ImGui::TreeNode("Configuration##2")) {

		ImGui::TreePop();
		ImGui::Spacing();
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	XMFLOAT4 m_vColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", (float*)&m_vColor);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();

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

unique_ptr<CMainApp> Client::CMainApp::Create() {

	auto pInstance = unique_ptr<CMainApp>(new CMainApp());

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to create MainApp");
	return pInstance;
}


