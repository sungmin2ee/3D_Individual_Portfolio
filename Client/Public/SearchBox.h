#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END


NS_BEGIN(Client)

class CSearchBox final : public CUIObject
{
public:
	typedef struct tagSearchDesc : public CUIObject::UIOBJECT_DESC
	{
		LEVEL nextLevel;
	}SEARCH_DESC;

private:
	CSearchBox(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CSearchBox(const CSearchBox& Prototype);
public:
	virtual ~CSearchBox();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:

	void Refresh() { m_bRefresh = true; }
	void Clear() { m_bClear = true; }
	_bool& Get_Render() { return m_bRender; }
private:
	shared_ptr<CVIBuffer_Rect>	m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>		m_pTextureCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_ItemFrames(LEVEL nextLevel);
public:
	static unique_ptr<CSearchBox> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	vector<pair<_float, _float>> framePos;
	vector<CGameObject> Icons;
	uint32_t itemCount = 0;
	_bool    m_bRefresh = false;
	_bool	 m_bClear = false;
};

NS_END



