#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END


NS_BEGIN(Client)

class CFade final : public CUIObject
{
public:

private:
	CFade(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CFade(const CFade& Prototype);
public:
	virtual ~CFade();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_bool &Get_Finished() { return m_bFinished; }
	void Set_Fade(uint32_t In0Out1) {
		if (In0Out1 == 0) {
			fadeIn = true;
			fadeOut = false;
		}
		else if (In0Out1 == 1) {
			fadeOut = true;
			fadeIn = false;
		}
		m_bFinished = false;
		m_bReset = true;
	}
private:
	shared_ptr<CVIBuffer_Rect>	m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>		m_pTextureCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();
	void Reset();

public:
	static unique_ptr<CFade> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	_float falpha = 1;
	_bool fadeIn = false;
	_bool fadeOut = false;
	_bool m_bReset = false;
	_bool m_bFinished = false;
};

NS_END



