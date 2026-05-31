#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END


NS_BEGIN(Client)

class CWeapon final : public CPartObject
{
public:
	typedef struct tagWeaponDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState = { nullptr };
		const _float4x4* pSocketMatrix = { nullptr };
	}WEAPON_DESC;

private:
	CWeapon(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CWeapon(const CWeapon& Prototype);
public:
	virtual ~CWeapon();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CModel>			m_pModelCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };

private:
	const uint32_t* m_pParentState = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static unique_ptr<CWeapon> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END