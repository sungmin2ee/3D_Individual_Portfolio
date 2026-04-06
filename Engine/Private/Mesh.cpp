#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> context, const vector<VERTEX>& vertices,
    const vector<UINT>& indices,  vector<Texture> textures)
    : vertices_(vertices), indices_(indices), textures_(textures), m_pDevice(dev), m_pContext(context)
{
    VertexBuffer_ = nullptr;
    IndexBuffer_ = nullptr;
    this->setupMesh(this->m_pDevice);
}

void Mesh::Draw()
{
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;

    m_pContext->IASetVertexBuffers(0, 1, VertexBuffer_.GetAddressOf(), &stride, &offset);
    m_pContext->IASetIndexBuffer(IndexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

    // Safety check
    if (!textures_.empty()) {
        m_pContext->PSSetShaderResources(0, 1, textures_[0].texture.GetAddressOf());
    }

    m_pContext->DrawIndexed(static_cast<UINT>(indices_.size()), 0, 0);
}

//shared_ptr<Mesh> Mesh::Create(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> context,
//    const vector<VERTEX>& vertices, const vector<UINT>& indices, const vector<Texture>& textures)
//{
//    auto		pInstance = shared_ptr<Mesh>(new Mesh(dev, context, vertices,indices,textures));
//
//    if (FAILED(pInstance->Initialize_Prototype()))
//    {
//        MSG_BOX("Failed to Created : CTransform");
//        return nullptr;
//    }
//
//    return pInstance;
//}

//shared_ptr<CPrototype> Mesh::Clone(void* pArg)
//{
//
//    shared_ptr<CPrototype> pClone = make_shared<Mesh>(*this);
//
//    if (pClone == nullptr)
//        return nullptr;
//
//    // 3. 만약 추가적인 인자(pArg)가 있다면 여기서 초기화 로직을 수행합니다.
//    // pClone->Initialize(pArg); 
//
//    return pClone;
//}

void Mesh::setupMesh(ComPtr<ID3D11Device> dev)
{
    {
        HRESULT hr;

        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = static_cast<UINT>(sizeof(VERTEX) * vertices_.size());
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = &vertices_[0];

        hr = dev->CreateBuffer(&vbd, &initData, VertexBuffer_.GetAddressOf());
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create vertex buffer.");
        }

        D3D11_BUFFER_DESC ibd;
        ibd.Usage = D3D11_USAGE_IMMUTABLE;
        ibd.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices_.size());
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        ibd.MiscFlags = 0;

        initData.pSysMem = &indices_[0];

        hr = dev->CreateBuffer(&ibd, &initData, IndexBuffer_.GetAddressOf());
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create index buffer.");
        }
    }
}

void Mesh::Release()
{
    vertices_.clear();
    indices_.clear();
    textures_.clear();
}
