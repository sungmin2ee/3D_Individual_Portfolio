#pragma once

#include "Engine_Defines.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLevel abstract
{
protected:
	CLevel(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

	void  Set_ChangeLevel() { m_bChangeLevel = true; }
	void  Set_NextLevel(uint32_t nextLevel) { m_NextLevel = nextLevel; }
	

protected:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };
	_bool							m_bChangeLevel = false;
	uint32_t						m_NextLevel = 0;
};

NS_END