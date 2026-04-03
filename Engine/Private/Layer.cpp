#include "Layer.h"

HRESULT CLayer::Add_GameObject(shared_ptr<CGameObject> pGameObject)
{
    if (nullptr == pGameObject)
        return E_FAIL;

    m_GameObjects.push_back(pGameObject);

    return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (nullptr != pGameObject)
            pGameObject->Priority_Update(fTimeDelta);
    }
}

void CLayer::Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (nullptr != pGameObject)
            pGameObject->Update(fTimeDelta);
    }
}

void CLayer::Late_Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_GameObjects)
    {
        if (nullptr != pGameObject)
            pGameObject->Late_Update(fTimeDelta);
    }
}

unique_ptr<CLayer> CLayer::Create()
{
    return unique_ptr<CLayer>(new CLayer());
}
