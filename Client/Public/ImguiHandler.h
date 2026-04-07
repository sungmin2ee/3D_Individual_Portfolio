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

	void Imgui_Logo(_float fTimeDelta);
	void Imgui_Loading(_float fTimeDelta);
	void Imgui_GamePlay(_float fTimeDelta);

public:
	static unique_ptr<CImguiHandler> Create(ENGINE_DESC desc);
private:
	ENGINE_DESC m_Desc;
};

NS_END