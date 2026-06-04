#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END


NS_BEGIN(Client)

class CInventory final : public CUIObject
{
public:
	typedef struct tagInventoryDesc : public CUIObject::UIOBJECT_DESC
	{
		LEVEL nextLevel;
	}INVENTORY_DESC;

private:
	CInventory(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CInventory(const CInventory& Prototype);
public:
	virtual ~CInventory();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	_bool& Get_Render() { return m_bRender; }

private:
	shared_ptr<CVIBuffer_Rect>	m_pVIBufferCom = { nullptr };
	shared_ptr<CTexture>		m_pTextureCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_ItemFrames(LEVEL nextLevel);
	void ReArrange();
public:
	static unique_ptr<CInventory> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	vector<pair<_float, _float>> framePos;
	vector<CGameObject> Icons;
	uint32_t itemCount = 0;
	_wstring m_selectedItemDesc;
	_wstring m_selectedItemName;
	_wstring m_selectedItemTag;
	_float	 m_fCellSizeX = 0;
	_float	 m_fCellSizeY = 0;

};

NS_END



