#pragma once

#include "Engine_Defines.h"

NS_BEGIN(Engine)

class  CImguiMgr
{
private:
	CImguiMgr(ENGINE_DESC EngineDesc);
public:
	virtual ~CImguiMgr();
public:
	HRESULT Ready_Imgui(HWND hWnd, ComPtr<ID3D11Device>& pOutDevice, ComPtr<ID3D11DeviceContext>& pOutContext);
	void Update_Imgui();
	HRESULT Render_Imgui();
	bool WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Before_DeviceReset();
	void After_DeviceReset();

	
	
public:
	static unique_ptr<CImguiMgr> Create(ENGINE_DESC EngineDesc);
private:
	virtual void	Free();
private:
	ENGINE_DESC engineDesc;
};

NS_END

