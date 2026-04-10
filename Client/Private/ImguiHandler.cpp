#include "ImguiHandler.h"
#include "GameInstance.h"
#include "Helper.h"
#include "Layer.h"
#include "Collider.h"

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

	//모든 오브젝트들을 가져와서 레이 피킹을 한다
	CLayer* layer1 = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::LOGO), L"Layer_Logo");
	list<shared_ptr<CGameObject>> objects = layer1->GetObjects();
	for (auto object : objects) {
	}

	_float3 rayPos;
	_float3 rayDir;
	CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);

	for (auto collider : CGameInstance::Get().Get_Colliders()) {
		shared_ptr<CCollider> pCollider = collider.lock();
		if (pCollider) {
			if (CGameInstance::Get().Mouse_Down(DIM_LB)) {
				if (pCollider->IntersectsRay())
				{
					CGameObject* object  =pCollider->GetOwner();
					//CGameObject* object = pCollider->
				}
			}
		}
	}
	//for (auto object : objects) {
	//	if()
	//}


	// 2. 카메라 행렬 및 오브젝트 월드 행렬 가져오기 (float*)
	//_float4x4 matWorld = pTargetObject->Get_Transform()->GetWorld();
	//_float4x4 view = CGameInstance::Get().GetView();
	//_float4x4 projection = CGameInstance::Get().GetProj();
	////float* view = (float*)&camera->GetViewMatrix();
	////float* projection = (float*)&camera->GetProjectionMatrix();
	////float* worldMatrix = (float*)&object->GetWorldMatrix();

	//// 3. 기즈모 그리기 및 조작 (이동 모드 예시)
	//ImGuizmo::Manipulate((float*)&view, (float*)&projection, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, (float*)&matWorld);

	//// 4. 조작 여부 확인
	//if (ImGuizmo::IsUsing()) {
	//	pTargetObject->Get_Transform()->Set_WorldMatrix(matWorld);

	//	// B. 행렬 분해를 통해 OBB용 데이터 추출
	//	XMMATRIX world = XMLoadFloat4x4(&matWorld);
	//	XMVECTOR vScale, vRotQuat, vTrans;

	//	// XMMatrixDecompose가 안 된다면 반드시 DirectX::를 붙여보세요.
	//	if (DirectX::XMMatrixDecompose(&vScale, &vRotQuat, &vTrans, world))
	//	{
	//		auto pObb = pTargetObject->Get_Obb();

	//		// 위치와 회전(Orientation) 업데이트
	//		XMStoreFloat3(&pObb->myOBB.Center, vTrans);
	//		XMStoreFloat4(&pObb->myOBB.Orientation, vRotQuat);

	//		// 스케일 변화가 있다면 Extents도 갱신
	//		_float3 vModelSize = pTargetObject->Get_Model()->Get_LocalSize(); // (max-min)*0.5f
	//		pObb->myOBB.Extents = XMFLOAT3(
	//			vModelSize.x * XMVectorGetX(vScale),
	//			vModelSize.y * XMVectorGetY(vScale),
	//			vModelSize.z * XMVectorGetZ(vScale)
	//		);
	//	}
	//}
	//static bool show_demo = true; // 프로그램이 꺼질 때까지 상태가 유지됨


	//if (show_demo)
	//{
	//	ImGui::ShowDemoWindow(&show_demo);
	//}
	_float fps = 1 / fTimeDelta;
	ImGui::Begin("My First Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선

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

