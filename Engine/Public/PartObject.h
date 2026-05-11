#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	typedef struct tagPartObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentMatrix;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CPartObject(const CPartObject& Prototype);
public:
	virtual ~CPartObject();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	const _float4x4*		m_pParentMatrix = { nullptr };
	_float4x4				m_CombinedWorldMatrix = { };

protected:
	void Make_CombinedWorldMatrix(_fmatrix ChildMatrix);
	HRESULT Bind_WorldMatrix(shared_ptr<class CShader> pShader, const _char* pConstantName);

public:
	virtual shared_ptr<CPrototype> Clone(void* pArg) = 0;

};

NS_END