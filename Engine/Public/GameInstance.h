#pragma once

#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameInstance
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
public:
	virtual ~CGameInstance();

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ComPtr<ID3D11Device>& pOutDevice, ComPtr<ID3D11DeviceContext>& pOutContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Draw();
	void Clear_Resources(uint32_t iClearLevelIndex);

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	void Compute_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region GRAHPIC_DEVICE
	HRESULT Clear_BackBuffer_View(const _float4* pClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT Change_Level(uint32_t iNewLevelIndex, unique_ptr<class CLevel> pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, unique_ptr<class CPrototype> pPrototype);
	shared_ptr<CPrototype> Clone_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region GAMEOBJECT_MANAGER
	HRESULT Add_GameObject_toLayer(uint32_t iPrototypeLevelIndex, const _wstring& strPrototypeTag,
		uint32_t iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, shared_ptr<CGameObject> pRenderObject);
#pragma endregion

#pragma region IMGUI_MANAGER
	bool WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#pragma endregion

#pragma region MODEL_LOADER
	unique_ptr<class Model> Load(string filename);
	void Close();
#pragma endregion

#pragma region CAMERA
	XMFLOAT4X4 GetView();
	XMFLOAT4X4 GetProj();
#pragma endregion
private:
	unique_ptr<class CGraphic_Device>				m_pGraphic_Device = { nullptr };
	unique_ptr<class CTimer_Manager>				m_pTimer_Manager = { nullptr };
	unique_ptr<class CLevel_Manager>				m_pLevel_Manager = { nullptr };
	unique_ptr<class CPrototype_Manager>			m_pPrototype_Manager = { nullptr };
	unique_ptr<class CObject_Manager>				m_pObject_Manager = { nullptr };
	unique_ptr<class CRenderer>						m_pRenderer = { nullptr };
	unique_ptr<class CImguiMgr>						m_pImguiMgr = { nullptr };
	unique_ptr<class ModelLoader>					m_pModelLoader = { nullptr };
	unique_ptr<class CCamera> 						m_pCamera = nullptr;

	ComPtr<ID3D11RasterizerState> m_pRasterizerState = { nullptr };

public:

public:
	void Release_Engine();

};

NS_END