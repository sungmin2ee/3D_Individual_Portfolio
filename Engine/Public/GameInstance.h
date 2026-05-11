#pragma once

#include "Prototype_Manager.h"
#include "Item_Manager.h"
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
	_float2 Get_ViewportSize() const {
		return m_vViewportSize;
	}
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
	uint32_t GetCurLevelIndex();
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, unique_ptr<class CPrototype> pPrototype);
	shared_ptr<CPrototype> Clone_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	CPrototype* Find_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag);
#pragma endregion

#pragma region GAMEOBJECT_MANAGER
	HRESULT Add_GameObject_toLayer(uint32_t iPrototypeLevelIndex, const _wstring& strPrototypeTag,
		uint32_t iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);

	class CLayer* Find_Layer(uint32_t iLayerLevelIndex, const _wstring& strLayerTag);
	map<const _wstring, unique_ptr<class CLayer>>* Get_Layers();
	uint32_t Get_NumLevels();
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, shared_ptr<CGameObject> pRenderObject);
#pragma endregion

#pragma region IMGUI_MANAGER
	bool WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Free();
#pragma endregion


#pragma region INPUT_MANAGER
	_byte	Get_DIKeyState(uint8_t byKeyID);

	_byte	Get_DIMouseState(DIMK eMouse);

	// 현재 마우스의 특정 축 좌표를 반환
	_long	Get_DIMouseMove(DIMM eMouseState);

	bool Key_Pressing(uint8_t byKeyID);
	bool Key_Up(uint8_t byKeyID);
	bool Key_Down(uint8_t byKeyID);

	bool Mouse_Pressing(DIMK eMouseState);
	bool Mouse_Up(DIMK eMouseState);
	bool Mouse_Down(DIMK eMouseState);
#pragma endregion



#pragma region HELPER
	void GetMousePointRay(_float3* pRayPos, _float3* pRayDir);
#pragma endregion

#pragma region COLLIDER_MANAGER
	void Add_Collider(weak_ptr<class CCollider> collider);
	vector<weak_ptr<class CCollider>>& Get_Colliders();
#pragma endregion

#pragma region SAVELOAD_MANAGER
	HRESULT Save(uint32_t level);
	HRESULT Load(uint32_t level);
#pragma endregion


#pragma region PIPELINE
	const _float4x4* Get_Transform(D3DTS eState);
	const _float4x4* Get_Transform_Inverse(D3DTS eState);
	const _float4* Get_CamPosition();
	void Set_Transform(D3DTS eState, _fmatrix TransformMatrix);
#pragma endregion

#pragma region ITEM_MANAGER
	map<_wstring, uint32_t>& Get_Items();
	void Add_Item(_wstring item);
	void Sub_Item(_wstring item);
	void Set_Changed(_bool flag);
	_bool Get_Changed();
	pair< _wstring, string>&  Get_WhichHow();
	vector<CItem_Manager::ITEMINFO_DESC>& Get_ItemInfo();
	
#pragma endregion

#pragma region FONT_MANAGER
	void Begin();
	void End();
	void RenderText(uint32_t fontIndex, const _wstring& text, _float posX, _float posY, _vector color, _float scale);
#pragma endregion


private:
	unique_ptr<class CGraphic_Device>				m_pGraphic_Device = { nullptr };
	unique_ptr<class CTimer_Manager>				m_pTimer_Manager = { nullptr };
	unique_ptr<class CLevel_Manager>				m_pLevel_Manager = { nullptr };
	unique_ptr<class CPrototype_Manager>			m_pPrototype_Manager = { nullptr };
	unique_ptr<class CObject_Manager>				m_pObject_Manager = { nullptr };
	unique_ptr<class CDInput_Manager>				m_pInput_Manager = { nullptr };
	unique_ptr<class CRenderer>						m_pRenderer = { nullptr };
	unique_ptr<class CImguiMgr>						m_pImguiMgr = { nullptr };
	unique_ptr<class CHelper> 						m_pHelper = nullptr;
	unique_ptr<class Collider_Manager> 				m_pCollider_Manager = nullptr;
	unique_ptr<class SaveLoad_Manager> 				m_pSaveLoad_Manager = nullptr;
	unique_ptr<class CPipeLine>						m_pPipeLine = { nullptr };
	unique_ptr<class CItem_Manager> 				m_pItem_Manager = nullptr;
	unique_ptr<class CFont_Manager> 				m_pFont_Manager = nullptr;


public:

public:
	void Release_Engine();

private:
	_float2											m_vViewportSize = {};

};

NS_END