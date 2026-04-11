#include "CImguiMgr.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GameInstance.h"
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CImguiMgr::CImguiMgr(ENGINE_DESC EngineDesc):engineDesc{EngineDesc}
{
}

CImguiMgr::~CImguiMgr()
{
	Free();
}

HRESULT CImguiMgr::Ready_Imgui(HWND hWnd, ComPtr<ID3D11Device>& pOutDevice, ComPtr<ID3D11DeviceContext>& pOutContext)
{
	//ImGui_ImplWin32_EnableDpiAwareness();
	//float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGuiStyle& style = ImGui::GetStyle();
	//style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	//style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
	//style.ScaleAllSizes

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.FontGlobalScale = main_scale;
	// TODO: 한글폰트 로딩하면 메모리많이쓰던데 정상인지확인 필요
	//	- 26.2.4 한글폰트 로딩 하면 176 메가점유, 로딩안하면 47 메가 점유
	// io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pOutDevice.Get(), pOutContext.Get());

	return S_OK;
}

void CImguiMgr::Update_Imgui()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGuiContext* pCtx = ImGui::GetCurrentContext();
	if (pCtx == nullptr) {
		// 여기가 nullptr이면 CreateContext가 실패했거나 컨텍스트가 날아간 것임
		return;
	}
	ImGui::SetCurrentContext(pCtx);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	/*ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	ImGuizmo::SetRect(0, 0, (_float)engineDesc.iWinSizeX, (_float)engineDesc.iWinSizeY);*/

}

HRESULT CImguiMgr::Render_Imgui()
{
	if (!ImGui::GetCurrentContext()) return E_FAIL;

	ImGui::Render();
	
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return S_OK;
}

bool CImguiMgr::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!ImGui::GetCurrentContext()) return false;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return false;
}

void CImguiMgr::Before_DeviceReset()
{
	if (!ImGui::GetCurrentContext()) return;
	
	ImGui_ImplDX11_InvalidateDeviceObjects();
}

void CImguiMgr::After_DeviceReset()
{
	if (!ImGui::GetCurrentContext()) return;
	ImGui_ImplDX11_CreateDeviceObjects();
}

unique_ptr<CImguiMgr> CImguiMgr::Create(ENGINE_DESC EngineDesc)
{

	return unique_ptr<CImguiMgr>(new CImguiMgr(EngineDesc));
}



void CImguiMgr::Free()
{
	if (!ImGui::GetCurrentContext()) return;

	// Cleanup
	
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
