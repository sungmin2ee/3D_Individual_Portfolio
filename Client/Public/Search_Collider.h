#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class Obb;
class VIBuffer_Collider;
NS_END

NS_BEGIN(Client)

class CSearch_Collider final : public CGameObject
{
public:


	typedef struct tagStairDesc: public GAMEOBJECT_DESC
	{
		_vector position;

	}SEARCH_COLLIDER_DESC;



private:
	CSearch_Collider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CSearch_Collider(const CSearch_Collider& Prototype);
public:
	virtual ~CSearch_Collider();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	shared_ptr<Obb> Get_Obb() { return m_pObbCom; };
	void Set_Render(_bool flag) { m_bRender = flag; }
private:
	shared_ptr<Obb>						m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider>		m_pObbBfCom = { nullptr };
	shared_ptr<CTexture>				m_pTextureCom = { nullptr };
	shared_ptr<CVIBuffer_Rect>			m_pVIBufferCom = { nullptr };
	shared_ptr<CShader>					m_pShaderCom = { nullptr };

	_bool								m_bRender = false;
private:
	HRESULT Ready_Components();
	void ExpandCollider();

public:
	static unique_ptr<CSearch_Collider> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

};

NS_END