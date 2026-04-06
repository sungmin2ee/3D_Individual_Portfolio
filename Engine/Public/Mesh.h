#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class ENGINE_DLL Mesh {
public:
    vector<VERTEX> vertices_;
    vector<UINT> indices_;
    vector<Texture> textures_;


    Mesh(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> context, const std::vector<VERTEX>& vertices,
        const std::vector<UINT>& indices, const std::vector<Texture> textures);
  

public:

    void Draw();

    //static shared_ptr<Mesh> Create(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> context,
    //    const vector<VERTEX>& vertices, const vector<UINT>& indices, const vector<Texture>& textures);

    //virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:

    void setupMesh(ComPtr<ID3D11Device> dev);
    void Release();
private:
    ComPtr<ID3D11Buffer>                    VertexBuffer_;
    ComPtr<ID3D11Buffer>                    IndexBuffer_;
    ComPtr<ID3D11Device>					m_pDevice = { nullptr };
    ComPtr<ID3D11DeviceContext>		        m_pContext = { nullptr };


    
};

NS_END