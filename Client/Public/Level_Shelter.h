#pragma once

#include "Client_Defines.h"
#include "Level.h"


NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)

class CLevel_Shelter final : public CLevel
{
private:
	CLevel_Shelter(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CLevel_Shelter();

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
virtual HRESULT Render() override; private:

private:
	HRESULT Ready_Layer_Inven(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Overlay(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
	HRESULT Ready_Layer_HealthUI(const _wstring& strLayerTag);
public:
	static unique_ptr<CLevel_Shelter> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pContext = nullptr;
};

NS_END