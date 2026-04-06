#include "Model.h"

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, const _wstring& strPrototypeTag)
    :CComponent(device,context), m_pDevice{ device }, m_pContext{ context }, prototypeTag{ strPrototypeTag }
{

}

Model::~Model()
{
    m_vMeshes.clear();
}
HRESULT Model::Initialize(void* pArg) {
    return S_OK;
}
HRESULT Model::Initialize_Prototype() {

    return S_OK;
}

unique_ptr<Model> Model::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _wstring& strPrototypeTag)
{
    auto	pInstance = unique_ptr<Model>(new Model(pDevice, pContext, strPrototypeTag));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBackGround");
        return nullptr;
    }

    return pInstance;
}


shared_ptr<CPrototype> Model::Clone(void* pArg)
{

    shared_ptr<CPrototype> pClone = make_shared<Model>(*this);

    if (pClone == nullptr)
        return nullptr;

    // 3. 만약 추가적인 인자(pArg)가 있다면 여기서 초기화 로직을 수행합니다.
    // pClone->Initialize(pArg); 

    return pClone;
}
void Model::Draw() {
    for (size_t i = 0; i < m_vMeshes.size(); ++i) {
        m_vMeshes[i].Draw();
    }
}