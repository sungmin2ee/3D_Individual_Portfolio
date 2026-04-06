#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Camera.h"
NS_BEGIN(Client)
class CMainApp
{
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

public:
	static unique_ptr<CMainApp> Create();

};
NS_END

