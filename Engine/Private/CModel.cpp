#include "CModel.h"

CModel::CModel(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
    : CComponent{ Prototype }
    , m_Meshes{ Prototype.m_Meshes }
{

}

CModel::~CModel()
{
    Free();
}

HRESULT CModel::Initialize_Prototype(const string pModelFilePath)
{

    namespace fs = std::filesystem;
    // 1. 파일 이름만 추출 (확장자 제외)
    // 1. bin 파일이 저장될 전용 폴더 지정
    string binRoot = "../../Resources/Models/BinData/";

    // 2. 파일명만 따오기 (Warrior)
    size_t lastSlash = pModelFilePath.find_last_of("/\\");
    size_t lastDot = pModelFilePath.find_last_of(".");
    string pureName = pModelFilePath.substr(lastSlash + 1, lastDot - (lastSlash + 1));

    // 3. 최종 bin 경로 생성
    string binPath = binRoot + pureName + ".bin";

    if (fs::exists(binPath)) {
        Ready_BinaryMeshes(binPath);
        //return LoadFromBinary(binPath); // 바이너리 폴더에서 읽기
    }
    else {

        uint32_t        iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_SortByPType |   //점 선 삼각형 타입별로 정리
            aiProcess_LimitBoneWeights | aiProcess_PopulateArmatureData | aiProcess_PreTransformVertices };

        m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
        if (nullptr == m_pAIScene)
            return E_FAIL;

        if (FAILED(Ready_Meshes()))
            return E_FAIL;

        // 메쉬 정보 
        std::ofstream fout;
        fout.open(binPath, std::ios::out | std::ios::binary);

        if (fout.is_open()) {
            uint32_t meshCount = static_cast<uint32_t>(m_Meshes.size());
            fout.write((const char*)&meshCount, sizeof(uint32_t));

            for (const auto& mesh : m_Meshes) {
                // 2. 이 메쉬의 정점 개수 저장
                uint32_t vertexCount = static_cast<uint32_t>(mesh->vertices_.size());
                fout.write((const char*)&vertexCount, sizeof(uint32_t));

                // 3. 정점 배열 통째로 저장 (가장 중요!)
                // 개별 x, y, z를 따로 쓰는 게 아니라 구조체 배열 전체를 한 번에 씁니다.
                fout.write((const char*)mesh->vertices_.data(), sizeof(VERTEX) * vertexCount);

                // 4. 인덱스 개수와 배열도 같은 방식으로 저장
                uint32_t indexCount = static_cast<uint32_t>(mesh->indices_.size());
                fout.write((const char*)&indexCount, sizeof(uint32_t));
                fout.write((const char*)mesh->indices_.data(), sizeof(uint32_t) * indexCount);
            }

        }

    }


    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render()
{
    for (auto& pMesh : m_Meshes)
    {
        pMesh->Bind_Resources();
        pMesh->Render();
    }

    return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        auto    pAIMesh = CMesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i]);
        if (nullptr == pAIMesh)
            return E_FAIL;

        m_Meshes.push_back(pAIMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_BinaryMeshes(string binPath)
{
    std::ifstream fin(binPath, std::ios::in | std::ios::binary);

    if (fin.is_open()) {
        // 1. 전체 메쉬 개수 읽기
        uint32_t meshCount = 0;
        fin.read((char*)&meshCount, sizeof(uint32_t));

        for (uint32_t i = 0; i < meshCount; ++i) {
            // 2. 이 메쉬의 정점 개수 읽기
            uint32_t vertexCount = 0;
            fin.read((char*)&vertexCount, sizeof(uint32_t));

            // 3. 정점 배열 읽기
            // 읽어올 크기만큼 벡터 메모리를 미리 확보합니다.
            vector<VTXMESH> vertices(vertexCount);
            fin.read((char*)vertices.data(), sizeof(VTXMESH) * vertexCount);

            // 4. 인덱스 개수와 배열 읽기
            uint32_t indexCount = 0;
            fin.read((char*)&indexCount, sizeof(uint32_t));

            vector<uint32_t> indices(indexCount);
            fin.read((char*)indices.data(), sizeof(uint32_t) * indexCount);

            // 5. 읽어온 데이터를 사용하여 CMesh 객체 생성
            // 이전에 만든 Create_Binary 혹은 전용 초기화 함수를 호출합니다.
            shared_ptr<CMesh> pMesh = CMesh::Create_Binary(m_pDevice, m_pContext, vertices, indices);

            if (pMesh != nullptr) {
                m_Meshes.push_back(pMesh);
            }
        }
        fin.close();
    }

    return S_OK;
}

unique_ptr<CModel> CModel::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, string pModelFilePath)
{
    auto		pInstance = unique_ptr<CModel>(new CModel(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype(pModelFilePath)))
    {
        MSG_BOX("Failed to Created : CModel");
        return nullptr;
    }

    return pInstance;
}


shared_ptr<CPrototype> CModel::Clone(void* pArg)
{
    auto		pInstance = shared_ptr<CModel>(new CModel(*this));

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CModel");
        return nullptr;
    }

    return pInstance;
}

void CModel::Free()
{
    m_Importer.FreeScene();
}
