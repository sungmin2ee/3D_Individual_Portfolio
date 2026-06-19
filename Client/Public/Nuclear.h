#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END


NS_BEGIN(Client)

class CNuclear final : public CGameObject
{
public:

private:
	CNuclear(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CNuclear(const CNuclear& Prototype);
public:
	virtual ~CNuclear();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Set_Start() { m_bStart = true; };

private:
	shared_ptr<CVIBuffer_Rect>	m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>		m_pTextureCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };
	
	uint32_t					m_iRow = 2;
	uint32_t					m_iCol = 12;
	uint32_t					m_iCurIndex = 0;
	_float						m_fEffectTime = 0;
	_bool						m_bStart = false;
private:
	HRESULT Ready_Components();
	HRESULT Ready_ItemFrames(LEVEL nextLevel);

public:
	static unique_ptr<CNuclear> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

};

NS_END



