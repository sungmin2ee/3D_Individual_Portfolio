#pragma once

#include "Client_Defines.h"
#include "Level.h"


NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)

class CLevel_Stage2 final : public CLevel
{
private:
	CLevel_Stage2(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CLevel_Stage2();

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
virtual HRESULT Render() override; private:

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Stair_Collider(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Inven(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Zombie(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Search_Collider(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Search_Box(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Overlay(const _wstring& strLayerTag);
	HRESULT Ready_Layer_HealthUI(const _wstring& strLayerTag);
	HRESULT Ready_BoxCollider(const _wstring& strLayerTag);
	HRESULT Ready_Fade(const _wstring& strLayerTag);

	HRESULT Load_Stair_Collider();
	HRESULT Load_Door_Blocker();

public:
	static unique_ptr<CLevel_Stage2> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	vector<shared_ptr<class CZombie>> m_vZombies;
	size_t workerCount = 0;
	shared_ptr<class CFade> fade = nullptr;

};

NS_END