#pragma once

#include "Client_Defines.h"
#include "Level.h"


NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)

class CLevel_Stage1 final : public CLevel
{
private:
	CLevel_Stage1(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CLevel_Stage1();

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
virtual HRESULT Render() override; private:

private:
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Zombie(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
public:
	static unique_ptr<CLevel_Stage1> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pContext = nullptr;
};

NS_END