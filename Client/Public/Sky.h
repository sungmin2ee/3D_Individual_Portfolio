#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Cube;
NS_END


NS_BEGIN(Client)

class CSky final : public CGameObject
{
public:
	typedef struct tagSkyDesc : public CGameObject::GAMEOBJECT_DESC
	{
		LEVEL nextLevel;

	}SKY_DESC;

private:
	CSky(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CSky(const CSky& Prototype);
public:
	virtual ~CSky();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CVIBuffer_Cube>		m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>			m_pTextureCom = { nullptr };
	shared_ptr<CShader>				m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static unique_ptr<CSky> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END