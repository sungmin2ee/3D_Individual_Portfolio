#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

NS_BEGIN(Engine)
	class CGameObject;
NS_END

NS_BEGIN(Client)
class CImguiHandler
{
private:
	CImguiHandler(ENGINE_DESC desc, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
public:
	virtual ~CImguiHandler();

public:
	void Handle_Imgui(uint32_t curlevel, _float fTimeDelta);
	void Initialize();
	void Imgui_Editor(_float fTimeDelta);


public:
	static unique_ptr<CImguiHandler> Create(ENGINE_DESC desc, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
private:
	ENGINE_DESC m_Desc;
	MODELOBJ_DESC m_ModelDesc;
	CGameObject* m_pSelected = nullptr;
	_float4 position = {};
	_float3 rotation = {};
	_float3 scale = {};
	ImGuizmo::OPERATION m_CurrentOperation;

	vector<string> prototypeTags;
	vector<string> filePath;
	vector<string> shaders;
	int m_SelectedIndex = -1;
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };
	_wstring strLayerTag = L"";
	MODEL modelType = {};
};

NS_END