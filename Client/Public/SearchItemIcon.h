#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END


NS_BEGIN(Client)

class CSearchItemIcon final : public CUIObject
{
public:
	typedef struct tagItemICon : public CUIObject::UIOBJECT_DESC
	{
		_bool    bInitialRender;
	}ITEMICON_DESC;

private:
	CSearchItemIcon(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CSearchItemIcon(const CSearchItemIcon& Prototype);
public:
	virtual ~CSearchItemIcon();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CVIBuffer_Rect>	m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>		m_pTextureCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components(_wstring iconTag);

public:
	static unique_ptr<CSearchItemIcon> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	_wstring itemIconTag = L"";

};

NS_END



