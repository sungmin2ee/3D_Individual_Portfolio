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

class CBoxCollider final : public CGameObject
{
public:

	enum class BOX {
		MAP, MOVE, FIX,EVENT ,END
	};
	typedef struct tagBoxDesc : public GAMEOBJECT_DESC
	{
		BOX		purpose;
		_vector position;

	}BOX_DESC;



private:
	CBoxCollider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CBoxCollider(const CBoxCollider& Prototype);
public:
	virtual ~CBoxCollider();

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
	BOX	&Get_Puspose() {
		return m_ePurpose;
	}
	void Set_Active() {
		m_bActive = true;
	}
	_bool Get_FirstActive() {
		return m_bFirstActive;
	}
private:
	shared_ptr<Obb>						m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider>		m_pObbBfCom = { nullptr };
	shared_ptr<CTexture>				m_pTextureCom = { nullptr };
	shared_ptr<CVIBuffer_Rect>			m_pVIBufferCom = { nullptr };
	shared_ptr<CShader>					m_pShaderCom = { nullptr };

	_bool								m_bRender = false;
	_bool								m_bActive = false;
	_bool								m_bLightOn = false;
	_bool								m_bFirstActive = true;
	BOX									m_ePurpose = BOX::END;
	shared_ptr<class CLight>			m_pLight = { nullptr };
	_float								m_pFlashTime = 0;
	uint32_t							m_iCount = 0;
private:
	HRESULT Ready_Components(BOX purpose);
	void ExpandCollider();

public:
	static unique_ptr<CBoxCollider> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

};

NS_END