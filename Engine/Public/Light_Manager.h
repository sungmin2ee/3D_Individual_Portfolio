#pragma once

#include "Engine_Defines.h"

NS_BEGIN(Engine)

class CLight_Manager final
{
private:
	CLight_Manager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CLight_Manager();

public:
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render(shared_ptr<class CShader> pShader, shared_ptr<class CVIBuffer_Rect> pVIBuffer);
	void Clear_Lights();
	list<shared_ptr<class CLight>> &Get_Lights() {
		return m_Lights;
	}
private:
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>			m_pContext = { nullptr };

	list<shared_ptr<class CLight>>		m_Lights;


public:
	static unique_ptr<CLight_Manager> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

};

NS_END