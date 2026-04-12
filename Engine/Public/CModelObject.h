#pragma once
#include "Engine_Defines.h"
#include "GameObject.h"
NS_BEGIN(Engine)

class ENGINE_DLL CModelObject :
    public CGameObject
{

private:
    CModelObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
    virtual ~CModelObject();

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Update(_float fTimeDelta);
    virtual void Update(_float fTimeDelta);
    virtual void Late_Update(_float fTimeDelta);
    virtual HRESULT Render();
protected:
    shared_ptr<class Model>   m_pModelCom = nullptr;
    shared_ptr<class Shader>  m_pShaderCom = nullptr;
    shared_ptr<class Obb> m_pColliderCom = nullptr;
    shared_ptr<class VIBuffer_Cube> m_pCubeBfCom = { nullptr };

    ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };
    ComPtr<ID3D11Device>            m_pDevice = { nullptr };
    // CGameObject을(를) 통해 상속됨

public:
    static unique_ptr<CModelObject> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
    shared_ptr<CPrototype> Clone(void* pArg) override;

};
NS_END
