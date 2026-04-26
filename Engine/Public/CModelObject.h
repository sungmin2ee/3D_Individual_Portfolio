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
    CModelObject(const CModelObject& Prototype);
    virtual ~CModelObject();

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Update(_float fTimeDelta) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render();

    void ExpandCollider();
protected:
    shared_ptr<class CModel>   m_pModelCom = nullptr;
    shared_ptr<class CShader>  m_pShaderCom = nullptr;
    shared_ptr<class Obb> m_pColliderCom = nullptr;
    // CGameObject을(를) 통해 상속됨

public:
    static unique_ptr<CModelObject> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
    shared_ptr<CPrototype> Clone(void* pArg) override;
private:
    //ComPtr<ID3D11SamplerState> m_pSamplerState;

};
NS_END
