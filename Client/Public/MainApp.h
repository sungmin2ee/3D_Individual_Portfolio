#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Camera.h"
NS_BEGIN(Client)
class CMainApp
{
    struct ImGuiDemoWindowData
    {
        // Examples Apps (accessible from the "Examples" menu)
        bool ShowMainMenuBar = false;
        bool ShowAppAssetsBrowser = false;
        bool ShowAppConsole = false;
        bool ShowAppCustomRendering = false;
        bool ShowAppDocuments = false;
        bool ShowAppLog = false;
        bool ShowAppLayout = false;
        bool ShowAppPropertyEditor = false;
        bool ShowAppSimpleOverlay = false;
        bool ShowAppAutoResize = false;
        bool ShowAppConstrainedResize = false;
        bool ShowAppFullscreen = false;
        bool ShowAppLongText = false;
        bool ShowAppWindowTitles = false;

        // Dear ImGui Tools (accessible from the "Tools" menu)
        bool ShowMetrics = false;
        bool ShowDebugLog = false;
        bool ShowIDStackTool = false;
        bool ShowStyleEditor = false;
        bool ShowAbout = false;

        // Other data
        bool DisableSections = false;

    };
private:
	CMainApp();
public:
	~CMainApp();

public:
	HRESULT Initialize();
	void Update(float fTimeDelta);
	HRESULT Render();

private:
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>			m_pContext = { nullptr };

private:
	HRESULT Start_Level(LEVEL eStartLevelIndex);
    unique_ptr<class CImguiHandler>		m_pImguiHandler = { nullptr };


public:
	static unique_ptr<CMainApp> Create();

};
NS_END

