#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class ENGINE_DLL SaveLoad_Manager
{
private:
	SaveLoad_Manager(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~SaveLoad_Manager();

public:

	HRESULT Initialize();
	HRESULT Save(uint32_t levelIndex);
	HRESULT Load(uint32_t levelIndex);
public:
	static unique_ptr<SaveLoad_Manager> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

private:
	_wstring strLayerTag = L"";
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };
};

NS_END
