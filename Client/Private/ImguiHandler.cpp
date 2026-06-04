#include "ImguiHandler.h"
#include "GameInstance.h"
#include "Helper.h"
#include "Layer.h"
#include "Collider.h"
#include "CModel.h"
#include "Door.h"
#include "Blocker.h"
#include "Stair_Collider.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

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
	
	Imgui_Editor(fTimeDelta);
	m_ModelDesc.levelIndex = curlevel;
	ImGui::Begin("Map Editor");

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		// 1. Model Tab
		if (ImGui::BeginTabItem("Model"))
		{
			static ImGuiTextFilter filter;
			filter.Draw("Model Filter");

			for (int i = 0; i < prototypeTags.size(); ++i)
			{
				if (filter.PassFilter(prototypeTags[i].c_str()))
				{
					ImGui::PushID(i);

					bool isSelected = (m_SelectedIndex == i);
					if (ImGui::Selectable(prototypeTags[i].c_str(), isSelected))
					{
						m_SelectedIndex = i;
						m_ModelDesc.pModelPrototypeTag = StringToWString(prototypeTags[i]);
						m_ModelDesc.filePath = filePath[i];
					}

					ImGui::PopID();
				}
			}
			ImGui::EndTabItem();
		}

		// 2. Shader Tab
		if (ImGui::BeginTabItem("Shader"))
		{
			static ImGuiTextFilter filter;
			filter.Draw("Shader Filter");

			for (int i = 0; i < shaders.size(); ++i)
			{
				if (filter.PassFilter(shaders[i].c_str()))
				{
					ImGui::PushID(i);

					bool isSelected = (m_SelectedIndex == i); // 필요에 따라 셰이더 전용 인덱스 변수를 쓰셔도 좋습니다.
					if (ImGui::Selectable(shaders[i].c_str(), isSelected))
					{
						m_SelectedIndex = i;
						m_ModelDesc.pShaderPrototypeTag = StringToWString(shaders[i]);
					}

					ImGui::PopID();
				}
			}
			ImGui::EndTabItem();
		}

		// 3. Collider Tab
		if (ImGui::BeginTabItem("Collider"))
		{
			if (ImGui::Button("Don't Collide")) { m_ModelDesc.collide = false; }
			ImGui::SameLine();
			if (ImGui::Button("Collide")) { m_ModelDesc.collide = true; }
			ImGui::EndTabItem();
		}

		// 4. ANIM? Tab
		if (ImGui::BeginTabItem("ANIM?"))
		{
			if (ImGui::Button("NON_ANIM"))
			{
				modelType = MODEL::NONANIM;
				m_ModelDesc.pShaderPrototypeTag = L"Prototype_Component_Shader_VtxMesh";
			}
			ImGui::SameLine();
			if (ImGui::Button("ANIM"))
			{
				modelType = MODEL::ANIM;
				m_ModelDesc.pShaderPrototypeTag = L"Prototype_Component_Shader_VtxAnimMesh";
			}
			ImGui::EndTabItem();
		}

		// 5. Layer Tab
		if (ImGui::BeginTabItem("Layer"))
		{
			if (ImGui::Button("UI_Layer")) { strLayerTag = L"UI_Layer"; }
			ImGui::SameLine();
			if (ImGui::Button("Outside_Layer")) { strLayerTag = L"Outside_Layer"; }
			ImGui::SameLine();
			if (ImGui::Button("Inside_Layer")) { strLayerTag = L"Inside_Layer"; }

			ImGui::Separator();

			if (ImGui::Button("Add_Object"))
			{
				_matrix camView, camWorld;
				const _float4x4* view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
				camView = XMLoadFloat4x4(view);
				camWorld = XMMatrixInverse(nullptr, camView);
				XMMATRIX scale = XMMatrixScaling(0.001f, 0.001f, 0.001f);
				XMMATRIX world = scale * camWorld;
				XMStoreFloat4x4(&m_ModelDesc.worldMatrix, world);
				_matrix PreTransformMatrix = XMMatrixIdentity();

				PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
				m_ModelDesc.pretransformMatrix = PreTransformMatrix;
				m_ModelDesc.modelType = ETOUI(modelType);

				if (CGameInstance::Get().Find_Prototype(m_ModelDesc.levelIndex, m_ModelDesc.pModelPrototypeTag) == nullptr) {
					auto pModelProto = CModel::Create(m_pDevice, m_pContext, m_ModelDesc.modelType, m_ModelDesc.filePath, m_ModelDesc.pretransformMatrix);
					CGameInstance::Get().Add_Prototype(curlevel, m_ModelDesc.pModelPrototypeTag, unique_ptr<CPrototype>(std::move(pModelProto)));
				}

				CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), L"Prototype_ModelObject", curlevel, strLayerTag, &m_ModelDesc);
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Stair"))
		{
			if (ImGui::Button("UP"))
			{
				UPorDown = ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_UP);
			}
			if (ImGui::Button("Down"))
			{
				UPorDown = ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_DOWN);

			}
			if (ImGui::Button("Add_Stair_Collider"))
			{
				CStair_Collider::STAIR_DESC desc;
				_matrix camView, camWorld;
				const _float4x4* view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
				camView = XMLoadFloat4x4(view);
				camWorld = XMMatrixInverse(nullptr, camView);
				XMMATRIX world = camWorld;
				XMStoreFloat4x4(&desc.worldMat, world);
				desc.pGameObjectTag = L"Stair_Collider";
				if (UPorDown == ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_UP)) {
					desc.state = CStair_Collider::STAIR_COLLIDER::STAIR_UP;

				}
				else if (UPorDown == ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_DOWN)) {
					desc.state = CStair_Collider::STAIR_COLLIDER::STAIR_DOWN;
				}
				_wstring layerTag = L"Layer_Stair_Collider";
				CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), L"Prototype_GameObject_Stair_Collider", curlevel, layerTag, &desc);
			}
			ImGui::EndTabItem();
		}

		// 6. Connect Door and Blocker Tab
		//if (ImGui::BeginTabItem("Connect_Door_and Blocker"))
		//{
		//	if (ImGui::Button("Create_Door"))
		//	{
		//		_matrix camView, camWorld;
		//		CDoor::DOOR_DESC pDesc{};
		//		_tchar szTag[32] = {};
		//		auto layer = CGameInstance::Get().Find_Layer(curlevel, L"Layer_Door");
		//		if (layer == nullptr) {
		//			swprintf_s(szTag, L"Door_%d", 0);
		//		}
		//		else {
		//			swprintf_s(szTag, L"Door_%d", layer->GetObjects().size());
		//		}
		//		pDesc.pGameObjectTag = szTag;
		//
		//		const _float4x4* view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
		//		camView = XMLoadFloat4x4(view);
		//		camWorld = XMMatrixInverse(nullptr, camView);
		//		XMMATRIX scale = XMMatrixScaling(0.001f, 0.001f, 0.001f);
		//		XMMATRIX world = scale * camWorld;
		//		XMStoreFloat4x4(&pDesc.worldMat, world);
		//
		//		pDesc.fRotationPerSec = 720.f;
		//		CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Door"), curlevel, L"Layer_Door", &pDesc);
		//	}
		//
		//	ImGui::SameLine();
		//
		//	if (ImGui::Button("Create_Blocker"))
		//	{
		//		_matrix camView, camWorld;
		//		CBlocker::BLOCKER_DESC pDesc{};
		//		_tchar szTag[32] = {};
		//		auto layer = CGameInstance::Get().Find_Layer(curlevel, L"Layer_Blocker");
		//
		//		if (layer == nullptr) {
		//			swprintf_s(szTag, L"Blocker_%d", 0);
		//		}
		//		else {
		//			uint32_t size = layer->GetObjects().size();
		//			swprintf_s(szTag, L"Blocker_%d", size);
		//		}
		//		pDesc.pGameObjectTag = szTag;
		//
		//		const _float4x4* view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
		//		camView = XMLoadFloat4x4(view);
		//		camWorld = XMMatrixInverse(nullptr, camView);
		//		XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		//		XMMATRIX world = scale * camWorld;
		//		XMStoreFloat4x4(&pDesc.worldMat, world);
		//
		//		pDesc.fRotationPerSec = 720.f;
		//		CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), TEXT("Prototype_GameObject_Blocker"), curlevel, L"Layer_Blocker", &pDesc);
		//	}
		//
		//	ImGui::Separator();
		//
		//	if (ImGui::Button("LeftDoor"))
		//	{
		//		static_pointer_cast<CDoor>(m_pCurDoor)->Set_RightBlocker(static_pointer_cast<CBlocker>(m_pCurBlocker));
		//		static_pointer_cast<CBlocker>(m_pCurBlocker)->Set_LeftDoor(static_pointer_cast<CDoor>(m_pCurDoor));
		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button("RightDoor"))
		//	{
		//		static_pointer_cast<CDoor>(m_pCurDoor)->Set_LeftBlocker(static_pointer_cast<CBlocker>(m_pCurBlocker));
		//		static_pointer_cast<CBlocker>(m_pCurBlocker)->Set_RightDoor(static_pointer_cast<CDoor>(m_pCurDoor));
		//	}
		//
		//	ImGui::EndTabItem();
		//} // 괄호를 여기로 옮겨서 Connect 탭을 완전히 닫아줍니다.
		//
		//// 7. Door Tab (이제 Connect 탭과 동등한 위치로 독립되었습니다)
		//if (ImGui::BeginTabItem("Door"))
		//{
		//	static ImGuiTextFilter filter1;
		//	filter1.Draw("Door Filter");
		//
		//	auto layer = CGameInstance::Get().Find_Layer(curlevel, L"Layer_Door");
		//	if (layer != nullptr) {
		//		auto doors = layer->GetObjects();
		//		uint32_t index = 0;
		//
		//		for (auto& door : doors) {
		//			std::string doorTag = WStringToString(door->Get_Tag());
		//
		//			if (filter1.PassFilter(doorTag.c_str())) {
		//				ImGui::PushID(index);
		//
		//				bool isSelected = (m_pSelected == door);
		//
		//				if (ImGui::Selectable(doorTag.c_str(), isSelected))
		//				{
		//					m_pSelected = door;
		//					m_pCurDoor = door;
		//				}
		//
		//				if (isSelected) {
		//					for (auto& collider : CGameInstance::Get().Get_Colliders()) {
		//						auto pCollider = collider.lock();
		//						pCollider->SetSelected(false);
		//					}
		//					for (auto& collider : CGameInstance::Get().Get_Colliders()) {
		//						auto pCollider = collider.lock();
		//						if (m_pSelected == pCollider->GetOwner()) {
		//							pCollider->SetSelected(true);
		//							break;
		//						}
		//					}
		//					ImGui::SetItemDefaultFocus();
		//				}
		//
		//				ImGui::PopID();
		//			}
		//			index++;
		//		}
		//	}
		//	ImGui::EndTabItem();
		//}
		//if (ImGui::BeginTabItem("Blocker"))
		//{
		//	static ImGuiTextFilter filter2;
		//	filter2.Draw("Blocker Filter");
		//
		//	auto layer = CGameInstance::Get().Find_Layer(curlevel, L"Layer_Blocker");
		//	if (layer != nullptr) {
		//		auto blockers = layer->GetObjects();
		//		uint32_t index = 0;
		//
		//		for (auto& blocker : blockers) {
		//			std::string blockerTag = WStringToString(blocker->Get_Tag());
		//
		//			if (filter2.PassFilter(blockerTag.c_str())) {
		//				ImGui::PushID(index);
		//
		//				bool isSelected = (m_pSelected == blocker);
		//
		//				if (ImGui::Selectable(blockerTag.c_str(), isSelected))
		//				{
		//					m_pSelected = blocker;
		//					m_pCurDoor = blocker;
		//				}
		//
		//				if (isSelected) {
		//					for (auto& collider : CGameInstance::Get().Get_Colliders()) {
		//						auto pCollider = collider.lock();
		//						pCollider->SetSelected(false);
		//					}
		//					for (auto& collider : CGameInstance::Get().Get_Colliders()) {
		//						auto pCollider = collider.lock();
		//						if (m_pSelected == pCollider->GetOwner()) {
		//							pCollider->SetSelected(true);
		//							break;
		//						}
		//					}
		//					ImGui::SetItemDefaultFocus();
		//				}
		//
		//				ImGui::PopID();
		//			}
		//			index++;
		//		}
		//	}
		//	ImGui::EndTabItem();
		//}
		ImGui::EndTabBar();
	}

	ImGui::End();

	// 저장 
	
}

void CImguiHandler::Imgui_Editor(_float fTimeDelta)
{


	_float3 rayPos;
	_float3 rayDir;
	
	_float fps = 1 / fTimeDelta;
	ImGui::Begin("My First Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선
	
	
	//CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);
	
	if (CGameInstance::Get().Mouse_Down(DIMK::RBUTTON)) {
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
					//auto blockerLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Blocker");
					//auto doorLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Door");
					//if (blockerLayer != nullptr) {
					//	auto blockers = blockerLayer->GetObjects();
					//	for (auto& blocker : blockers) {
					//		if (m_pSelected == blocker) {
					//			m_pCurBlocker = m_pSelected;
					//			break;
					//		}
					//	}
					//}
					//if (doorLayer != nullptr) {
					//	auto doors = doorLayer->GetObjects();
					//	for (auto& door : doors) {
					//		if (m_pSelected == door) {
					//			m_pCurDoor = m_pSelected;
					//			break;
					//		}
					//	}
					//}
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
	
	ImGuizmo::BeginFrame();
	
	ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
	ImGuizmo::SetRect(0, 0, g_iWinSizeX, g_iWinSizeY);
	if (m_pSelected) {
	
		if (CGameInstance::Get().Key_Down('1')) m_CurrentOperation = ImGuizmo::TRANSLATE;
		if (CGameInstance::Get().Key_Down('2')) m_CurrentOperation = ImGuizmo::ROTATE;
		if (CGameInstance::Get().Key_Down('3')) m_CurrentOperation = ImGuizmo::SCALE;
	
		_float4x4 matWorld = m_pSelected->Get_Transform()->GetWorld();
		const _float4x4* view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
		const _float4x4* projection = CGameInstance::Get().Get_Transform(D3DTS::PROJ);
		ImGuizmo::Manipulate(
			(float*)view,
			(float*)projection,
			m_CurrentOperation,
			ImGuizmo::LOCAL,
			(float*)&matWorld
		);
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
	
	
	
	if (ImGui::CollapsingHeader("Transform")) {
		if (m_pSelected) {
			_float3 sca = m_pSelected->Get_Transform()->Get_Scaled();
	
			// ImGuizmo 세팅 (매 프레임 호출 필수)
			
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
	
		
		}
	}
	
	
	
	ImGui::End();
	
	
			
	if (m_pSelected) {
		if (CGameInstance::Get().Key_Pressing(DIK_LCONTROL)) {
			if (CGameInstance::Get().Key_Down(DIK_V)) {
				MODELOBJ_DESC desc = m_pSelected->Get_Desc();
				//desc.worldMatrix = m_pSelected->Get_Transform()->GetWorld();
				CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::STATIC), L"Prototype_ModelObject", CGameInstance::Get().GetCurLevelIndex(), strLayerTag, &desc);
	
			}
		}
		if (CGameInstance::Get().Key_Down(DIK_DELETE)) {
			m_pSelected->Set_Dead();
			m_pSelected = nullptr;
		}
	}
	
	
	
	// 2. 카메라 행렬 및 오브젝트 월드 행렬 가져오기 (float*)
	
	//static bool show_demo = true; // 프로그램이 꺼질 때까지 상태가 유지됨
	//
	//
	//if (show_demo)
	//{
	//	ImGui::ShowDemoWindow(&show_demo);
	//}
	


}

//void CImguiHandler::Save_DoorAndBlocker()
//{
//	json Main;
//	auto layers = CGameInstance::Get().Get_Layers();
//	uint32_t levelIndex = CGameInstance::Get().GetCurLevelIndex();
//	map<const _wstring, unique_ptr<class CLayer>>::iterator iter = layers[levelIndex].begin();
//
//	for (iter; iter != layers[levelIndex].end(); iter++) {
//		if (iter->first == L"Layer_Door" || iter->first == L"Layer_Blocker") {
//			for (auto object : iter->second->GetObjects()) {
//				json j;
//
//				MODELOBJ_DESC desc = object->Get_Desc();
//				_vector right, up, look, pos;
//
//				right = object->Get_Transform()->Get_State(STATE::RIGHT);
//				up = object->Get_Transform()->Get_State(STATE::UP);
//				look = object->Get_Transform()->Get_State(STATE::LOOK);
//				pos = object->Get_Transform()->Get_State(STATE::POSITION);
//
//				_float4 rightf, upf, lookf, posf;
//				XMStoreFloat4(&rightf, right);
//				XMStoreFloat4(&upf, up);
//				XMStoreFloat4(&lookf, look);
//				XMStoreFloat4(&posf, pos);
//				_wstring prototypeTag,tag, leftTag, rightTag;
//				if (iter->first == L"Layer_Door") {
//					prototypeTag = L"Prototype_GameObject_Door";
//					tag =static_pointer_cast<CDoor>(object)->Get_Tag();
//					auto door = static_pointer_cast<CDoor>(object);
//					auto leftBlocker = door->Get_LeftBlocker();
//					auto rightBlocker = door->Get_RightBlocker();
//					if (leftBlocker == nullptr) {
//						leftTag = L"Empty";
//					}
//					else {
//						leftTag = door->Get_LeftBlocker()->Get_Tag();
//					}
//					if (rightBlocker == nullptr) {
//						rightTag = L"Empty";
//					}
//					else {
//						rightTag = door->Get_RightBlocker()->Get_Tag();
//					}
//				}
//				else if (iter->first == L"Layer_Blocker") {
//					prototypeTag = L"Prototype_GameObject_Blocker";
//					tag = static_pointer_cast<CBlocker>(object)->Get_Tag();
//					auto blocker = static_pointer_cast<CBlocker>(object);
//					auto leftDoor = blocker->Get_LeftDoor();
//					auto rightDoor= blocker->Get_RightDoor();
//					if (leftDoor == nullptr) {
//						leftTag = L"Empty";
//					}
//					else {
//						leftTag = blocker->Get_LeftDoor()->Get_Tag();
//					}
//					if (rightDoor == nullptr) {
//						rightTag = L"Empty";
//					}
//					else {
//						rightTag = blocker->Get_RightDoor()->Get_Tag();
//					}
//
//
//				}
//				j["PrototypeTag"] = WStringToString(prototypeTag);
//				j["ObjectTag"] = WStringToString(tag);
//				j["Layer"] = WStringToString(iter->first);
//				j["LeftObject"] = WStringToString(leftTag);
//				j["RightObject"] = WStringToString(rightTag);
//				j["Right"] = { rightf.x,rightf.y, rightf.z, rightf.w };
//				j["Up"] = { upf.x, upf.y, upf.z, upf.w };
//				j["Look"] = { lookf.x, lookf.y, lookf.z, lookf.w };
//				j["Position"] = { posf.x, posf.y, posf.z, posf.w };
//
//				Main["GameObjects"].push_back(j);
//			}
//		}
//	}
//	string LevelName = "";
//	switch (levelIndex) {
//	case 2:
//		LevelName = "LOGO";
//		break;
//	case 3:
//		LevelName = "SHELTER";
//
//		break;
//	case 4:
//		LevelName = "STAGE1";
//		break;
//	case 5:
//		LevelName = "STAGE2";
//		break;
//	}
//
//	string savePath1 = "../../Resources/Data/" + LevelName + "_DoorAndBlocker.json";
//	std::ofstream file1(savePath1);
//	if (file1.is_open()) {
//		file1 << Main.dump(4); // 4는 들여쓰기(Tab) 간격입니다.
//		file1.close();
//	}
//
//
//
//}


HRESULT CImguiHandler::Load_DoorAndBlocker()
{
	//m_pSelected = nullptr;
	//m_pCurDoor = nullptr;
	//m_pCurBlocker = nullptr;
	string LevelName = "";
	uint32_t levelIndex = CGameInstance::Get().GetCurLevelIndex();

	switch (levelIndex) {
	case 2:
		LevelName = "LOGO";
		break;
	case 3:
		LevelName = "SHELTER";

		break;
	case 4:
		LevelName = "STAGE1";
		break;
	case 5:
		LevelName = "STAGE2";
		break;
	}

	string path = "../../Resources/Data/" + LevelName + "_DoorAndBlocker.json";
	ifstream file(path);
	if (!file.is_open()) {
		return E_FAIL;
	}

	json j;
	file >> j;

	for (auto& gameObject : j["GameObjects"])
	{
		_wstring prototypeTag, layerTag;
		CDoor::DOOR_DESC desc;
		prototypeTag = StringToWString(gameObject.value("PrototypeTag", ""));
		desc.pGameObjectTag = StringToWString(gameObject.value("ObjectTag", ""));
		layerTag = StringToWString(gameObject.value("Layer", ""));
		desc.leftTag = StringToWString(gameObject.value("LeftObject", ""));
		desc.rightTag = StringToWString(gameObject.value("RightObject", ""));

		_float fRight[4], fUp[4], fLook[4], fPos[4];
		for (int i = 0; i < 4; ++i) {
			fRight[i] = gameObject["Right"][i].get<float>();
			fUp[i] = gameObject["Up"][i].get<float>();
			fLook[i] = gameObject["Look"][i].get<float>();
			fPos[i] = gameObject["Position"][i].get<float>();
		}
		XMMATRIX matWorld = XMMatrixIdentity();

		matWorld.r[0] = XMLoadFloat4((_float4*)fRight);  // Right
		matWorld.r[1] = XMLoadFloat4((_float4*)fUp);     // Up
		matWorld.r[2] = XMLoadFloat4((_float4*)fLook);   // Look
		matWorld.r[3] = XMLoadFloat4((_float4*)fPos);    // Position
		XMStoreFloat4x4(&desc.worldMat, matWorld);

		if (prototypeTag == L"Prototype_GameObject_Blocker") {
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Blocker", levelIndex, layerTag, &desc))) {
				return E_FAIL;
			}
		}
		else if (prototypeTag == L"Prototype_GameObject_Door") {
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Door", levelIndex, layerTag, &desc))) {
				return E_FAIL;
			}
		}
	}

	auto blockerLayer = CGameInstance::Get().Find_Layer(levelIndex, L"Layer_Blocker");
	auto doorLayer =CGameInstance::Get().Find_Layer(levelIndex, L"Layer_Door");

	auto blockers = blockerLayer->GetObjects();
	auto doors = doorLayer->GetObjects();
	for (auto& blocker : blockers) {
		auto block = static_pointer_cast<CBlocker>(blocker);
		if (block->Get_LeftTag() != L"Empty") {
			for (auto& door : doors) {
				auto pDoor = static_pointer_cast<CDoor>(door);
				if (block->Get_LeftTag() == pDoor->Get_Tag()) {
					block->Set_LeftDoor(pDoor);
					break;
				}
			}
		}
		if (block->Get_RightTag()!= L"Empty") {
			for (auto& door : doors) {
				auto pDoor = static_pointer_cast<CDoor>(door);
				if (block->Get_RightTag() == pDoor->Get_Tag()) {
					block->Set_RightDoor(pDoor);
					break;
				}
			}
		}
	}
	for (auto& door : doors) {
		auto pDoor = static_pointer_cast<CDoor>(door);
		if (pDoor->Get_LeftTag() != L"Empty") {
			for (auto& blocker : blockers) {
				auto pBlocker = static_pointer_cast<CBlocker>(blocker);
				if (pDoor->Get_LeftTag() == pBlocker->Get_Tag()) {
					pDoor->Set_LeftBlocker(pBlocker);
					break;
				}
			}
		}
		if (pDoor->Get_RightTag() != L"Empty") {
			for (auto& blocker : blockers) {
				auto pBlocker = static_pointer_cast<CBlocker>(blocker);
				if (pDoor->Get_RightTag() == pBlocker->Get_Tag()) {
					pDoor->Set_RightBlocker(pBlocker);
					break;
				}
			}
		}
	}
	return S_OK;
}
void CImguiHandler::Save_StairCollider()
{
	json Main;
	auto layers = CGameInstance::Get().Get_Layers();
	uint32_t levelIndex = CGameInstance::Get().GetCurLevelIndex();
	map<const _wstring, unique_ptr<class CLayer>>::iterator iter = layers[levelIndex].begin();

	for (iter; iter != layers[levelIndex].end(); iter++) {
		if (iter->first == L"Layer_Stair_Collider") {
			for (auto object : iter->second->GetObjects()) {
				json j;
				auto stairCollider = static_pointer_cast<CStair_Collider>(object);

				_vector right, up, look, pos;

				right = object->Get_Transform()->Get_State(STATE::RIGHT);
				up = object->Get_Transform()->Get_State(STATE::UP);
				look = object->Get_Transform()->Get_State(STATE::LOOK);
				pos = object->Get_Transform()->Get_State(STATE::POSITION);

				_float4 rightf, upf, lookf, posf;
				XMStoreFloat4(&rightf, right);
				XMStoreFloat4(&upf, up);
				XMStoreFloat4(&lookf, look);
				XMStoreFloat4(&posf, pos);
				uint32_t upOrDown = ETOUI(stairCollider->Get_State());
				
				j["UpOrDown"] = upOrDown;
				j["Layer"] = WStringToString(iter->first);
				j["Right"] = { rightf.x,rightf.y, rightf.z, rightf.w };
				j["Up"] = { upf.x, upf.y, upf.z, upf.w };
				j["Look"] = { lookf.x, lookf.y, lookf.z, lookf.w };
				j["Position"] = { posf.x, posf.y, posf.z, posf.w };

				Main["GameObjects"].push_back(j);
				
			}
		}
	}
	string LevelName = "";
	switch (levelIndex) {
	case 2:
		LevelName = "LOGO";
		break;
	case 3:
		LevelName = "SHELTER";

		break;
	case 4:
		LevelName = "STAGE1";
		break;
	case 5:
		LevelName = "STAGE2";
		break;
	}

	string savePath1 = "../../Resources/Data/" + LevelName + "_Stair_Collider.json";
	std::ofstream file1(savePath1);
	if (file1.is_open()) {
		file1 << Main.dump(4); // 4는 들여쓰기(Tab) 간격입니다.
		file1.close();
	}



}

HRESULT CImguiHandler::Load_StairCollider()
{
	string LevelName = "";
	uint32_t levelIndex = CGameInstance::Get().GetCurLevelIndex();

	switch (levelIndex) {
	case 2:
		LevelName = "LOGO";
		break;
	case 3:
		LevelName = "SHELTER";

		break;
	case 4:
		LevelName = "STAGE1";
		break;
	case 5:
		LevelName = "STAGE2";
		break;
	}

	string path = "../../Resources/Data/" + LevelName + "_Stair_Collider.json";
	ifstream file(path);
	if (!file.is_open()) {
		return E_FAIL;
	}

	json j;
	file >> j;

	for (auto& gameObject : j["GameObjects"])
	{
		_wstring layerTag;
		CStair_Collider::STAIR_DESC desc;
		layerTag = StringToWString(gameObject.value("Layer", ""));
		uint32_t uState = gameObject["UpOrDown"].get<uint32_t>();
		if (uState == ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_UP)) {
			desc.state = CStair_Collider::STAIR_COLLIDER::STAIR_UP;
		}
		else if (uState == ETOUI(CStair_Collider::STAIR_COLLIDER::STAIR_DOWN)) {
			desc.state = CStair_Collider::STAIR_COLLIDER::STAIR_DOWN;
		}
		_float fRight[4], fUp[4], fLook[4], fPos[4];
		for (int i = 0; i < 4; ++i) {
			fRight[i] = gameObject["Right"][i].get<float>();
			fUp[i] = gameObject["Up"][i].get<float>();
			fLook[i] = gameObject["Look"][i].get<float>();
			fPos[i] = gameObject["Position"][i].get<float>();
		}
		XMMATRIX matWorld = XMMatrixIdentity();

		matWorld.r[0] = XMLoadFloat4((_float4*)fRight);  // Right
		matWorld.r[1] = XMLoadFloat4((_float4*)fUp);     // Up
		matWorld.r[2] = XMLoadFloat4((_float4*)fLook);   // Look
		matWorld.r[3] = XMLoadFloat4((_float4*)fPos);    // Position
		XMStoreFloat4x4(&desc.worldMat, matWorld);
		desc.pGameObjectTag = L"Stair_Collider";
		if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_GameObject_Stair_Collider", levelIndex, layerTag, &desc))) {
			return E_FAIL;
		}
	
	}
	return S_OK;
}



unique_ptr<CImguiHandler> CImguiHandler::Create(ENGINE_DESC desc, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context) {

	return  unique_ptr<CImguiHandler>(new CImguiHandler(desc, device, context));

}

