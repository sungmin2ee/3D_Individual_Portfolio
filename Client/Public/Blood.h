#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Particle_Point;
NS_END


NS_BEGIN(Client)

class CBlood final : public CPartObject
{

public:
	typedef struct tagBloodDesc : public CPartObject::PARTOBJECT_DESC
	{
		shared_ptr<class CZombie> zombie;
	}BLOOD_DESC;
private:
	CBlood(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CBlood(const CBlood& Prototype);
public:
	virtual ~CBlood();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Play_Particle();
	void Set_Bleeding() { m_bBleeding = true; }

private:
	shared_ptr<CVIBuffer_Particle_Point>	m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>				m_pTextureCom = { nullptr };
	shared_ptr<CShader>					m_pShaderCom = { nullptr };
	_bool								m_bBleeding = false;
	weak_ptr<class CZombie>           m_pZombie;
private:
	HRESULT Ready_Components();

public:
	static unique_ptr<CBlood> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END