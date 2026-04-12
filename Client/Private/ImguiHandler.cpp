#include "ImguiHandler.h"
#include "GameInstance.h"
#include "Helper.h"
#include "Layer.h"
#include "Collider.h"

CImguiHandler::CImguiHandler(ENGINE_DESC desc): m_Desc(desc)
{
	m_CurrentOperation = ImGuizmo::TRANSLATE;

}

CImguiHandler::~CImguiHandler()
{
}
void CImguiHandler::Initialize(uint32_t curlevel,const string& strSceneName) {
	modelsName.clear();
	using json = nlohmann::json;

	string path = "../../Resources/Data/" + strSceneName + "_List.json";
	std::ifstream file(path);

	if (!file.is_open())
	{
		return;
	}
	json j;
	file >> j;
	string nodeName = "";
	nodeName = "Scene_" + strSceneName;
	for (auto& model : j[nodeName]["Model"])
	{
		bool isAnim = model.value("IsAnim", false);
		modelsName.push_back(model.value("Model", ""));
	}
}
void CImguiHandler::Handle_Imgui(uint32_t curlevel, _float fTimeDelta)
{

	if (!loaded && curlevel > ETOUI(LEVEL::LOADING)) {
		curLevel = CGameInstance::Get().GetCurLevelIndex();
		switch (curlevel) {
			case ETOUI(LEVEL::LOGO):
			{
				SceneName = "Logo";
				break;
			}
			case ETOUI(LEVEL::GAMEPLAY): {
				SceneName = "Gameplay";
				break;
			}
		}
		Initialize(curlevel, SceneName);
		loaded = true;
	}
	if (curLevel != curlevel) {
		loaded = false;
	}
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

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		char buffer[64] = "";
		if (ImGui::BeginTabItem("Model"))
		{
			ImGui::InputText("UTF-8 input", buffer, IM_COUNTOF(buffer));
			
			for (int i = 0; i < modelsName.size(); ++i)
			{
				ImGui::PushID(i);  

				ImGui::Text("%s", modelsName[i].c_str());

				if (ImGui::Button("Select"))
				{
					m_SelectedIndex = i;
					m_ModelDesc.pModelPrototypeTag = StringToWString(modelsName[i]);
				}

				ImGui::PopID();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Shader"))
		{
			ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Collider"))
		{
			ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void CImguiHandler::Imgui_Logo(_float fTimeDelta)
{


	_float3 rayPos;
	_float3 rayDir;

	_float fps = 1 / fTimeDelta;
	ImGui::Begin("My First Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선

	
	CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);

	if (CGameInstance::Get().Mouse_Down(DIM_RB)) {
		_float fMinDist = 100000.f;
		for (auto& collider : CGameInstance::Get().Get_Colliders()) {
			auto pCollider = collider.lock();
			if (pCollider) pCollider->SetSelected(false); // 모두 초록색으로 초기화
		}

		for (auto& collider : CGameInstance::Get().Get_Colliders()) {
			auto pCollider = collider.lock();
			_float fDist = 0.f;
			if (pCollider && pCollider->IntersectsRay(&fDist)) {
				if (fDist < fMinDist) {
					fMinDist = fDist;
					m_pSelected = pCollider->GetOwner();
				}
			}
		}
		for (auto& collider : CGameInstance::Get().Get_Colliders()) {
			auto pCollider = collider.lock();
			if (m_pSelected == pCollider->GetOwner()) {
				pCollider->SetSelected(true);
				XMStoreFloat4(&position, m_pSelected->Get_Transform()->Get_State(STATE::POSITION));

			}
		}

	}
	//for (auto& collider : CGameInstance::Get().Get_Colliders()) {
	//	auto pCollider = collider.lock();
	//	if (CGameInstance::Get().Mouse_Down(DIM_LB)) {
	//		if (pCollider && pCollider->IntersectsRay()) {
	//			m_pSelected = pCollider->GetOwner();
	//			break; // 하나 찾았으면 중단
	//		}
	//	}
	//}


	if (ImGui::CollapsingHeader("Transform")) {
		if (m_pSelected) {

			if (CGameInstance::Get().Key_Down('1')) m_CurrentOperation = ImGuizmo::TRANSLATE;
			if (CGameInstance::Get().Key_Down('2')) m_CurrentOperation = ImGuizmo::ROTATE;
			if (CGameInstance::Get().Key_Down('3')) m_CurrentOperation = ImGuizmo::SCALE;
			// ImGuizmo 세팅 (매 프레임 호출 필수)
			ImGuizmo::BeginFrame();
			ImGuizmo::SetRect(0, 0, g_iWinSizeX, g_iWinSizeY); // 화면 해상도에 맞게
			_float4x4 matWorld = m_pSelected->Get_Transform()->GetWorld();
			_float4x4 view = CGameInstance::Get().GetView();
			_float4x4 projection = CGameInstance::Get().GetProj();
			
			bool bModified = false;
			bModified |= ImGui::DragFloat("Pos X", &position.x, 0.1f);
			bModified |= ImGui::DragFloat("Pos Y", &position.y, 0.1f);
			bModified |= ImGui::DragFloat("Pos Z", &position.z, 0.1f);
			bModified |= ImGui::DragFloat("Rot X", &rotation.x, 0.1f);
			bModified |= ImGui::DragFloat("Rot Y", &rotation.y, 0.1f);
			bModified |= ImGui::DragFloat("Rot Z", &rotation.z, 0.1f);


			if (bModified) {
		

				m_pSelected->Get_Transform()->Set_State(STATE::POSITION, XMLoadFloat4(&position));
				m_pSelected->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), rotation.x);
				m_pSelected->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), rotation.y);
				m_pSelected->Get_Transform()->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f),rotation.z);
				rotation = { 0.f,0.f,0.f };

			}
			// 기즈모 조작
			ImGuizmo::Manipulate((float*)&view, (float*)&projection, m_CurrentOperation, ImGuizmo::LOCAL, (float*)&matWorld);

			if (ImGuizmo::IsUsing()) {
				m_pSelected->Get_Transform()->SetWorld(matWorld);
			
			}
		}
	}

	ImGui::End();


			
		



	// 2. 카메라 행렬 및 오브젝트 월드 행렬 가져오기 (float*)
	
	static bool show_demo = true; // 프로그램이 꺼질 때까지 상태가 유지됨


	if (show_demo)
	{
		ImGui::ShowDemoWindow(&show_demo);
	}
	


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

