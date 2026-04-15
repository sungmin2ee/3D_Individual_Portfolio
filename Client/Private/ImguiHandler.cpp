#include "ImguiHandler.h"
#include "GameInstance.h"
#include "Helper.h"
#include "Layer.h"
#include "Collider.h"
#include "Model.h"

CImguiHandler::CImguiHandler(ENGINE_DESC desc, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context): m_Desc(desc), m_pDevice{device},m_pContext{context}
{
	m_CurrentOperation = ImGuizmo::TRANSLATE;

}

CImguiHandler::~CImguiHandler()
{
}
void CImguiHandler::Initialize() {
	prototypeTags.clear();
	filePath.clear();
	shaders.clear();
	using json = nlohmann::json;

	string path = "../../Resources/Data/Model_List.json";
	std::ifstream file(path);

	if (!file.is_open())
	{
		return;
	}
	json j;
	file >> j;

	for (auto& model : j["Model"])
	{
		prototypeTags.push_back(model.value("PrototypeTag", ""));
		filePath.push_back(model.value("FilePath", ""));
	}
	

	string path1 = "../../Resources/Data/Shader_List.json";
	std::ifstream file1(path1);

	if (!file1.is_open())
	{
		return;
	}
	json j1;
	file1 >> j1;


	for (auto& shader : j1["Shader"])
	{
		shaders.push_back(shader.value("PrototypeTag", ""));
	}
}
void CImguiHandler::Handle_Imgui(uint32_t curlevel, _float fTimeDelta)
{
	
	switch (curlevel) {
	case ETOUI(LEVEL::LOGO):
		strLayerTag = L"Layer_Logo";
		Imgui_Logo(fTimeDelta);
		break;
	
	case ETOUI(LEVEL::GAMEPLAY): 
		strLayerTag = L"Layer_GamePlay";
		Imgui_GamePlay(fTimeDelta);
		break;
	
	}
	m_ModelDesc.levelIndex = curlevel;

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Model"))
		{
			static ImGuiTextFilter filter;
			filter.Draw("Model Filter");

			for (int i = 0; i < prototypeTags.size(); ++i)
			{
				ImGui::PushID(i);

				if (filter.PassFilter(prototypeTags[i].c_str()))
				{
					ImGui::Selectable(prototypeTags[i].c_str());

					if (ImGui::Button("Select"))
					{
						m_SelectedIndex = i;
						m_ModelDesc.pModelPrototypeTag = StringToWString(prototypeTags[i]);
						m_ModelDesc.filePath = filePath[i];
					}
				}

				ImGui::PopID();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Shader"))
		{
			static ImGuiTextFilter filter;
			filter.Draw("Shader Filter");

			for (int i = 0; i < shaders.size(); ++i)
			{
				ImGui::PushID(i);
				if (filter.PassFilter(shaders[i].c_str()))
				{
					ImGui::Selectable(shaders[i].c_str());

					if (ImGui::Button("Select"))
					{
						m_SelectedIndex = i;
						m_ModelDesc.pShaderPrototypeTag = StringToWString(shaders[i]);
					}
				}

				ImGui::PopID();
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Collider"))
		{
			if (ImGui::Button("Don't Collide"))
			{
				m_ModelDesc.collide = false;
			}
			if (ImGui::Button("Collide"))
			{
				m_ModelDesc.collide = true;
			}
			ImGui::EndTabItem();
		}

		//ADD OBJECT 버튼을 누르면 모델 프로토타입을 추가후 클론 
		if (ImGui::Button("Add_Object")) {
			_matrix view, camWorld;
			view = CGameInstance::Get().GetViewXM();
			camWorld = XMMatrixInverse(nullptr, view);
			XMStoreFloat4x4(&m_ModelDesc.worldMatrix, camWorld);
			auto pModelProto = Model::Create(m_pDevice, m_pContext, m_ModelDesc.filePath);

			CGameInstance::Get().Add_Prototype(curlevel, m_ModelDesc.pModelPrototypeTag, unique_ptr<CPrototype>(std::move(pModelProto)));
			//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
//	ETOUI(LEVEL::LOGO), strLayerTag, &Desc)))
//	return E_FAIL;
			CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), L"Prototype_ModelObject", curlevel, strLayerTag, &m_ModelDesc);

			//uint32_t levelIndex;
			//string filePath;
			//_wstring pModelPrototypeTag; // "Prototype_Component_Model_Desk"
			//_wstring pShaderPrototypeTag; // "Prototype_Component_Shader_NonAnim"
			//_float4x4  worldMatrix;
			//_bool   AddCollider;

		}



		ImGui::EndTabBar();
	}

	// 저장 
	
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
				scale = m_pSelected->Get_Transform()->Get_Scaled();

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
			_float3 sca = m_pSelected->Get_Transform()->Get_Scaled();

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
			bModified |= ImGui::DragFloat("Scale X", &scale.x, 0.001f);
			bModified |= ImGui::DragFloat("Scale Y", &scale.y, 0.001f);
			bModified |= ImGui::DragFloat("Scale Z", &scale.z, 0.001f);

			
			if (bModified) {
		
				m_pSelected->Get_Transform()->Scale_Non_Cumulate(scale.x,scale.y,scale.z);
				m_pSelected->Get_Transform()->Set_Rotation(rotation);
				m_pSelected->Get_Transform()->Set_State(STATE::POSITION, XMLoadFloat4(&position));
				//m_pSelected->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), rotation.x);
				//m_pSelected->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), rotation.y);
				//m_pSelected->Get_Transform()->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f),rotation.z);
				//rotation = { 0.f,0.f,0.f };

			}
			// 기즈모 조작
			ImGuizmo::Manipulate((float*)&view, (float*)&projection, m_CurrentOperation, ImGuizmo::LOCAL, (float*)&matWorld);

			if (ImGuizmo::IsUsing()) {
				// 1. 기즈모로 변한 matWorld를 실제 트랜스폼에 적용
				m_pSelected->Get_Transform()->SetWorld(matWorld);

				// 2. 중요: 변한 행렬에서 Scale, Rotation, Translation을 다시 추출해서 ImGui 변수에 동기화
				_float3 vScale, vRotation, vPos;
				ImGuizmo::DecomposeMatrixToComponents((float*)&matWorld, (float*)&vPos, (float*)&vRotation, (float*)&vScale);

				// ImGui와 연결된 변수들 갱신
				position = _float4(vPos.x, vPos.y, vPos.z, 1.f);
				rotation = vRotation; // (ImGuizmo는 Degree 값을 줍니다)
				scale = vScale;
				_float3 sca = m_pSelected->Get_Transform()->Get_Scaled();
			
			}
		}
	}



	ImGui::End();


			
	if (m_pSelected) {
		if (CGameInstance::Get().Key_Pressing(DIK_LCONTROL)) {
			if (CGameInstance::Get().Key_Down(DIK_V)) {
				MODELOBJ_DESC desc = m_pSelected->Get_Desc();
				desc.worldMatrix = m_pSelected->Get_Transform()->GetWorld();
				CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), L"Prototype_ModelObject", CGameInstance::Get().GetCurLevelIndex(), strLayerTag, &desc);

			}
		}
	}



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

unique_ptr<CImguiHandler> CImguiHandler::Create(ENGINE_DESC desc, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context) {

	return  unique_ptr<CImguiHandler>(new CImguiHandler(desc, device, context));

}

