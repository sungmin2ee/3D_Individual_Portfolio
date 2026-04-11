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
/* _float fps = 1 / fTimeDelta;
	ImGui::Begin("My Second Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선
	if (ImGui::TreeNode("Configuration##2")) {

		/*ImGui::Begin("Operation");
		if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
			m_CurrentGizmoOperation = ImGuizmo::SCALE;

ImGui::TreePop();
ImGui::Spacing();
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) {  }
			if (ImGui::MenuItem("Save", "Ctrl+S")) {  }
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
*/
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




	// B. 행렬 분해를 통해 OBB용 데이터 추출
	//XMMATRIX world = XMLoadFloat4x4(&matWorld);
	//XMVECTOR vScale, vRotQuat, vTrans;

	//// XMMatrixDecompose가 안 된다면 반드시 DirectX::를 붙여보세요.
	//if (DirectX::XMMatrixDecompose(&vScale, &vRotQuat, &vTrans, world))
	//{
	//	auto pObb = object->Get_Obb();

	//	// 위치와 회전(Orientation) 업데이트
	//	XMStoreFloat3(&pObb->myOBB.Center, vTrans);
	//	XMStoreFloat4(&pObb->myOBB.Orientation, vRotQuat);

	//	// 스케일 변화가 있다면 Extents도 갱신
	//	_float3 vModelSize = object->Get_Model()->Get_LocalSize(); // (max-min)*0.5f
	//	pObb->myOBB.Extents = XMFLOAT3(
	//		vModelSize.x * XMVectorGetX(vScale),
	//		vModelSize.y * XMVectorGetY(vScale),
	//		vModelSize.z * XMVectorGetZ(vScale)
	//	);
	//}
	//for (auto object : objects) {
	//	if()
	//}


	// 2. 카메라 행렬 및 오브젝트 월드 행렬 가져오기 (float*)
	
	static bool show_demo = true; // 프로그램이 꺼질 때까지 상태가 유지됨


	if (show_demo)
	{
		ImGui::ShowDemoWindow(&show_demo);
	}
	

	//if (CGameInstance::Get().Mouse_Pressing(DIM_LB)) {
	//	_float3 rayPos;
	//	_float3 rayDir;
	//	CHelper::GetMousePointRay(&rayPos, &rayDir);
	//}
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

