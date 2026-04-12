#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
NS_BEGIN(Client)
class CImguiHandler
{
private:
	CImguiHandler(ENGINE_DESC desc);
public:
	~CImguiHandler();

public:
	void Handle_Imgui(uint32_t curlevel, _float fTimeDelta);
	void Initialize(uint32_t curlevel, const string& strSceneName);
	void Imgui_Logo(_float fTimeDelta);
	void Imgui_Loading(_float fTimeDelta);
	void Imgui_GamePlay(_float fTimeDelta);

public:
	static unique_ptr<CImguiHandler> Create(ENGINE_DESC desc);
private:
	ENGINE_DESC m_Desc;
	MODELOBJ_DESC m_ModelDesc;
	CGameObject* m_pSelected = nullptr;
	_float4 position = {};
	_float3 rotation = {};
	ImGuizmo::OPERATION m_CurrentOperation;

	vector<string> modelsName;
	bool loaded = false;
	uint32_t curLevel;
	string SceneName;
	int m_SelectedIndex = -1;
};

NS_END