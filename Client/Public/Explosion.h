#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END


NS_BEGIN(Client)

class CExplosion final : public CGameObject
{


private:
	CExplosion(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CExplosion(const CExplosion& Prototype);
public:
	virtual ~CExplosion();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Set_Start() {
		m_bStart = true;
	}

private:
	shared_ptr<CModel>			m_pModelCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static unique_ptr<CExplosion> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	_bool m_bStart = false;
	_bool m_bSoundStart = false;
};

NS_END