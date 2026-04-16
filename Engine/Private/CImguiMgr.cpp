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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGuiStyle& style = ImGui::GetStyle();


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    

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
		return;
	}
	ImGui::SetCurrentContext(pCtx);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


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
