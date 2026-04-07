#include "ImguiHandler.h"
#include "GameInstance.h"
#include "Helper.h"

CImguiHandler::CImguiHandler(ENGINE_DESC desc): m_Desc(desc)
{
}

CImguiHandler::~CImguiHandler()
{
}

void CImguiHandler::Handle_Imgui(uint32_t curlevel, _float fTimeDelta)
{
	switch (curlevel) {
	case ETOUI(LEVEL::LOGO):
	{
		Imgui_Logo(fTimeDelta);
	}
	break;
	case ETOUI(LEVEL::GAMEPLAY): {
		Imgui_GamePlay(fTimeDelta);
	}
	break;
	}
}

void CImguiHandler::Imgui_Logo(_float fTimeDelta)
{
	//static bool show_demo = true; // 프로그램이 꺼질 때까지 상태가 유지됨


	//if (show_demo)
	//{
	//	ImGui::ShowDemoWindow(&show_demo);
	//}
	_float fps = 1 / fTimeDelta;
	ImGui::Begin("My First Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선

	if (CGameInstance::Get().Mouse_Pressing(DIM_LB)) {
		_float3 rayPos;
		_float3 rayDir;
		CHelper::GetMousePointRay(m_Desc, &rayPos, &rayDir);
	}
	//if (ImGui::TreeNode("gUIZMO")) {

	//	ImGui::TreePop();
	//	ImGui::Spacing();
	//}
	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}
	//XMFLOAT4 m_vColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	//// Edit a color (stored as ~4 floats)
	//ImGui::ColorEdit4("Color", (float*)&m_vColor);

	//// Plot some values
	//const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	//ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	//// Display contents in a scrolling region
	//ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	//ImGui::BeginChild("Scrolling");
	//for (int n = 0; n < 50; n++)
	//	ImGui::Text("%04d: Some text", n);
	//ImGui::EndChild();
	ImGui::End();
}

void CImguiHandler::Imgui_Loading(_float fTimeDelta)
{
}

void CImguiHandler::Imgui_GamePlay(_float fTimeDelta)
{
}

unique_ptr<CImguiHandler> CImguiHandler::Create(ENGINE_DESC desc) {

	return  unique_ptr<CImguiHandler>(new CImguiHandler(desc));

}

