#include "SaveLoad_Manager.h"
#include "GameInstance.h"
#include "CModel.h"
#include "Layer.h"
namespace fs = std::filesystem;
using json = nlohmann::json;

SaveLoad_Manager::SaveLoad_Manager(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext): m_pDevice{pDevice},m_pContext{pContext}
{
}

SaveLoad_Manager::~SaveLoad_Manager()
{
}

HRESULT SaveLoad_Manager::Initialize()
{
	
	return S_OK;
}

HRESULT SaveLoad_Manager::Save(uint32_t levelIndex)
{
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
	
	json Main;
	auto layers = CGameInstance::Get().Get_Layers();
	
	map<const _wstring, unique_ptr<class CLayer>>::iterator iter = layers[levelIndex].begin();
	for (iter; iter != layers[levelIndex].end(); iter++) {
		if (iter->first != L"Layer_Camera" && iter->first != L"Layer_UI"&& iter->first != L"UI_EquipBorder"&& iter->first != L"UI_ICons"
			&& iter->first != L"Layer_Door" && iter->first != L"Layer_Blocker") {
			for (auto object : iter->second->GetObjects()) {
				//월드 행렬
				json j;

				MODELOBJ_DESC desc = object->Get_Desc();
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
				j["PrototypeTag"] = WStringToString(desc.pModelPrototypeTag);
				j["FilePath"] = desc.filePath;
				j["ShaderTag"] = WStringToString(desc.pShaderPrototypeTag);
				j["Layer"] = WStringToString(iter->first);
				j["Collide"] = desc.collide;
				j["ModelType"] = desc.modelType;
				j["LevelIndex"] = desc.levelIndex;
				j["Right"] = { rightf.x,rightf.y, rightf.z, rightf.w };
				j["Up"] = { upf.x, upf.y, upf.z, upf.w };
				j["Look"] = { lookf.x, lookf.y, lookf.z, lookf.w };
				j["Position"] = { posf.x, posf.y, posf.z, posf.w };

				//오브젝트 이름 예: Prototype_Model_Joe5
				Main["GameObjects"].push_back(j);
			}
		}
		
	}
	//CLayer* layer = CGameInstance::Get().Find_Layer(levelIndex, strLayerTag);
	
	
	string savePath1 = "../../Resources/Data/" + LevelName + "_GameObjects.json";
	std::ofstream file1(savePath1);
	if (file1.is_open()) {
		file1 << Main.dump(4); // 4는 들여쓰기(Tab) 간격입니다.
		file1.close();
	}


	

	return S_OK;
}

HRESULT SaveLoad_Manager::Load(uint32_t levelIndex)
{
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

	string path = "../../Resources/Data/" + LevelName + "_GameObjects.json";
	ifstream file(path);
	if (!file.is_open()) {
		return E_FAIL;
	}

	json j;
	file >> j;

	MODELOBJ_DESC desc;
	for (auto& gameObject : j["GameObjects"])
	{
		desc.levelIndex = levelIndex;
		desc.pModelPrototypeTag = StringToWString(gameObject.value("PrototypeTag", ""));
		desc.filePath = gameObject.value("FilePath", "");
		desc.pShaderPrototypeTag = StringToWString(gameObject.value("ShaderTag", ""));
		strLayerTag = StringToWString(gameObject.value("Layer", ""));
		desc.collide = gameObject["Collide"].get<bool>();
		desc.modelType = gameObject["ModelType"].get<float>();

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
		XMStoreFloat4x4(&desc.worldMatrix,matWorld);
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(180.f));
		//XMMATRIX matWorld1 = XMLoadFloat4x4(&desc.worldMatrix);
		//XMMATRIX matResult = XMMatrixIdentity();
		//matResult = matRot* matWorld1;
		//XMStoreFloat4x4(&desc.worldMatrix, matResult);
		desc.pretransformMatrix= XMMatrixIdentity();

		desc.pretransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
		//모델 프로토타입이 있나?
		//모델부터 로드해야됨 
		if (CGameInstance::Get().Find_Prototype(desc.levelIndex, desc.pModelPrototypeTag) == nullptr) {
			auto pModelProto = CModel::Create(m_pDevice, m_pContext,desc.modelType, desc.filePath,desc.pretransformMatrix);
			if (FAILED(CGameInstance::Get().Add_Prototype(levelIndex, desc.pModelPrototypeTag, unique_ptr<CPrototype>(std::move(pModelProto))))) {
				return E_FAIL;
			}
		}

		
		if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(0, L"Prototype_ModelObject", levelIndex, strLayerTag, &desc))) {
			return E_FAIL;
		}
	}

	return S_OK;
}
unique_ptr<SaveLoad_Manager> SaveLoad_Manager::Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext)
{

	auto		pInstance = unique_ptr<SaveLoad_Manager>(new SaveLoad_Manager(pDevice, pContext));

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : SaveLoad_Manager");
		return nullptr;
	}

	return pInstance;
}

																					 