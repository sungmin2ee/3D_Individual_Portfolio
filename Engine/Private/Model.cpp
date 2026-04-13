#include "Model.h"
#include "GameInstance.h"
#include "../../Client/Public/Player.h"

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, const string& filename)
    :CComponent(device,context), m_pDevice{ device }, m_pContext{ context }
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

void Model::Update_Box()
{
    Calculate_Box();
}

void Model::Calculate_Box()
{
    if (m_vMeshes.empty() || m_vMeshes[0].vertices_.empty())
        return;


    _float3 firstPos = m_vMeshes[0].vertices_[0].position;
    _float minX = firstPos.x, minY = firstPos.y, minZ = firstPos.z;
    _float maxX = firstPos.x, maxY = firstPos.y, maxZ = firstPos.z;

    for (auto& mesh : m_vMeshes) {
        for (auto& vertex : mesh.vertices_) {
            if (vertex.position.x < minX) {
                minX = vertex.position.x;
            }
            if (vertex.position.y < minY) {
                minY = vertex.position.y;
            }
            if (vertex.position.z < minZ) {
                minZ = vertex.position.z;
            }
            if (vertex.position.x > maxX) {
                maxX = vertex.position.x;
            }
            if (vertex.position.y > maxY) {
                maxY = vertex.position.y;
            }
            if (vertex.position.z > maxZ) {
                maxZ = vertex.position.z;
            }
        }
    }
    max = { maxX, maxY, maxZ};
    min = { minX, minY, minZ};
}

unique_ptr<Model> Model::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const string& filename)
{

    auto	pInstance = unique_ptr<Model>(new Model(pDevice, pContext, filename));
    
    CGameInstance::Get().Load(filename);
    pInstance->Set_Meshes(std::move(CGameInstance::Get().Get_Meshes()));

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


void Model::Draw(CShader* pShader) {
    // 1. 월드 행렬 바인딩

    for (auto& mesh : m_vMeshes) {
        // 2. 메쉬 고유의 텍스처 바인딩 (Mesh에 Get_Diffuse() 등이 있다고 가정)
        // 현재 Mesh::Draw 내부에 PSSetShaderResources가 있지만 
        // CShader의 Bind_Texture를 사용하는 것이 일관성에 좋습니다.
        if (!mesh.textures_.empty()) {
            pShader->Bind_Texture("g_DiffuseTexture", mesh.textures_[0].texture);
        }

        // 3. 변경된 텍스처/상태를 GPU에 적용
        pShader->Apply_Pass();

        // 4. Vertex/Index Buffer 세팅 및 DrawIndexed
        mesh.Draw();
    }
}