    #include "CModel.h"

    CModel::CModel(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
        : CComponent{ pDevice, pContext }
    {
    }

    CModel::CModel(const CModel& Prototype)
        : CComponent{ Prototype }
        , m_eModelType{ Prototype.m_eModelType }
        , m_iNumMeshes{ Prototype.m_iNumMeshes }
        , m_Meshes{ Prototype.m_Meshes }
        , m_iNumMaterials{ Prototype.m_iNumMaterials }
        , m_Materials{ Prototype.m_Materials }
        , m_Bones{ Prototype.m_Bones }
        , m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
    {

    }

    CModel::~CModel()
    {
        Free();
    }

    int32_t CModel::Get_BoneIndex(const _char* pBoneName)
    {
        int32_t iBoneIndex = { 0 };
        auto    iter = find_if(m_Bones.begin(), m_Bones.end(), [&](shared_ptr<CBone> pBone)->_bool
            {
                if (true == pBone->Compare_Name(pBoneName))
                    return true;

                ++iBoneIndex;

                return false;
            });

        if (iter == m_Bones.end())
            return -1;

        return iBoneIndex;
    }

    HRESULT CModel::Initialize_Prototype(uint32_t eModelType, const _string& strModelFilePath, _fmatrix PreTransformMatrix)
    {
        m_eModelType = eModelType;

        namespace fs = std::filesystem;
        // 1. 파일 이름만 추출 (확장자 제외)
        // 1. bin 파일이 저장될 전용 폴더 지정
        string binRoot = "../../Resources/Models/Binary/";

        // 2. 파일명만 따오기 (Warrior)
        size_t lastSlash = strModelFilePath.find_last_of("/\\");
        size_t lastDot = strModelFilePath.find_last_of(".");
        string pureName = strModelFilePath.substr(lastSlash + 1, lastDot - (lastSlash + 1));

        // 3. 최종 bin 경로 생성
        string binPath = binRoot + pureName + ".bin";

        if (fs::exists(binPath)) {
            Ready_BinaryMeshes(binPath, m_eModelType);
            //return LoadFromBinary(binPath); // 바이너리 폴더에서 읽기
        }
        else {

            uint32_t        iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_SortByPType |   //점 선 삼각형 타입별로 정리
                aiProcess_LimitBoneWeights | aiProcess_PopulateArmatureData };

            if (ETOUI(MODEL::NONANIM) == m_eModelType)
                iFlag |= aiProcess_PreTransformVertices;


            m_pAIScene = m_Importer.ReadFile(strModelFilePath, iFlag);
            if (nullptr == m_pAIScene)
                return E_FAIL;

            XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

            Ready_Bones(m_pAIScene->mRootNode, -1);

            if (FAILED(Ready_Meshes()))
                return E_FAIL;

            if (FAILED(Ready_Materials(strModelFilePath)))
                return E_FAIL;


   
            // 메쉬 정보 
            std::ofstream fout;
            fout.open(binPath, std::ios::out | std::ios::binary);

            if (fout.is_open()) {
                uint32_t meshCount = static_cast<uint32_t>(m_Meshes.size());
                fout.write((const char*)&meshCount, sizeof(uint32_t));

                if (eModelType == 0) {
                    for (const auto& mesh : m_Meshes) {
                        // 2. 이 메쉬의 정점 개수 저장
                        uint32_t vertexCount = static_cast<uint32_t>(mesh->nonAnim_vertices.size());
                        fout.write((const char*)&vertexCount, sizeof(uint32_t));

                        // 3. 정점 배열 통째로 저장 (가장 중요!)
                        // 개별 x, y, z를 따로 쓰는 게 아니라 구조체 배열 전체를 한 번에 씁니다.
                        fout.write((const char*)mesh->nonAnim_vertices.data(), sizeof(VTXMESH) * vertexCount);

                        // 4. 인덱스 개수와 배열도 같은 방식으로 저장
                        uint32_t indexCount = static_cast<uint32_t>(mesh->indices_.size());
                        fout.write((const char*)&indexCount, sizeof(uint32_t));
                        fout.write((const char*)mesh->indices_.data(), sizeof(uint32_t) * indexCount);

                        uint32_t iMaterialIndex = mesh->Get_MaterialIndex(); // CMesh에 인덱스 반환 함수가 있다고 가정
                        fout.write((const char*)&iMaterialIndex, sizeof(uint32_t));
                    }
                }
                else if (eModelType == 1) {
                    for (const auto& mesh : m_Meshes) {
                        // 2. 이 메쉬의 정점 개수 저장
                        uint32_t vertexCount = static_cast<uint32_t>(mesh->Anim_vertices.size());
                        fout.write((const char*)&vertexCount, sizeof(uint32_t));

                        // 3. 정점 배열 통째로 저장 (가장 중요!)
                        // 개별 x, y, z를 따로 쓰는 게 아니라 구조체 배열 전체를 한 번에 씁니다.
                        fout.write((const char*)mesh->Anim_vertices.data(), sizeof(VTXANIMMESH) * vertexCount);

                        // 4. 인덱스 개수와 배열도 같은 방식으로 저장
                        uint32_t indexCount = static_cast<uint32_t>(mesh->indices_.size());
                        fout.write((const char*)&indexCount, sizeof(uint32_t));
                        fout.write((const char*)mesh->indices_.data(), sizeof(uint32_t) * indexCount);

                        uint32_t iMaterialIndex = mesh->Get_MaterialIndex(); // CMesh에 인덱스 반환 함수가 있다고 가정
                        fout.write((const char*)&iMaterialIndex, sizeof(uint32_t));
                    }

                    //uint32_t boneCount = static_cast<uint32_t>(m_Bones.size());
                    //fout.write((const char*)&boneCount, sizeof(uint32_t));

                    //for (auto& pBone : m_Bones) {
                    //    // 1. 이름 저장
                    //    string strName = pBone->Get_Name();
                    //    uint32_t iLen = (uint32_t)strName.length();
                    //    fout.write((char*)&iLen, sizeof(uint32_t));
                    //    fout.write(strName.c_str(), iLen);

                    //    // 2. 부모 인덱스 저장
                    //    int32_t iParentIndex = pBone->Get_ParentIndex();
                    //    fout.write((char*)&iParentIndex, sizeof(int32_t));

                    //    // 3. 변환 행렬들 저장 (Transformation, OffsetMatrix)
                    //    _float4x4 matTransform = pBone->Get_OffsetMatrix(); // OffsetMatrix
                    //    fout.write((char*)&matTransform, sizeof(_float4x4));

                    //    // 4. 로컬 행렬도 저장 (애니메이션 초기 상태용)
                    //    _float4x4 matLocal = pBone->Get_TransformationMatrix();
                    //    fout.write((char*)&matLocal, sizeof(_float4x4));
                    //}
                }
               

                uint32_t materialCount = static_cast<uint32_t>(m_Materials.size());
                fout.write((const char*)&materialCount, sizeof(uint32_t));

                for (auto& pMaterial : m_Materials) {
                    // 2. 각 타입(Diffuse~Max)을 순회
                    for (uint32_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i) {
                        // 3. 해당 타입의 텍스처 개수 기록
                        uint32_t iTextureCount = pMaterial->Get_TextureCount(i);
                        fout.write((char*)&iTextureCount, sizeof(uint32_t));

                        for (uint32_t j = 0; j < iTextureCount; ++j) {
                            // 4. 미리 저장해둔 경로 문자열 기록
                            string strPath = pMaterial->Get_TexturePath(i, j);
                            uint32_t iLen = (uint32_t)strPath.length();

                            fout.write((char*)&iLen, sizeof(uint32_t));
                            fout.write(strPath.c_str(), iLen);
                        }
                    }
                }

            }

        }


        return S_OK;
    }

    HRESULT CModel::Initialize(void* pArg)
    {
        return S_OK;
    }
    void CModel::Play_Animation(_float fTimeDelta)
    {
        /* 뼈들의 m_TransformationMatrix를 갱신해준다. */

        for (auto& pBone : m_Bones)
        {
            pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
        }

    }
    HRESULT CModel::Render(uint32_t iMeshIndex)
    {
   
        m_Meshes[iMeshIndex]->Bind_Resources();
        m_Meshes[iMeshIndex]->Render();

        return S_OK;
    }
    HRESULT CModel::Bind_Materials(shared_ptr<class CShader> pShader, const _char* pConstantName, uint32_t iMeshIndex, aiTextureType eMaterialType, uint32_t iTextureIndex)
    {

        return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()]->Bind_ShaderResource(pShader, pConstantName, eMaterialType, iTextureIndex);

    }
    HRESULT CModel::Bind_BoneMatrices(shared_ptr<class CShader> pShader, const _char* pConstantName, uint32_t iMeshIndex)
    {
        return m_Meshes[iMeshIndex]->Bind_BoneMatrices(m_Bones, pShader, pConstantName);

    }
    HRESULT CModel::Ready_Meshes()
    {
        m_iNumMeshes = m_pAIScene->mNumMeshes;

        for (size_t i = 0; i < m_iNumMeshes; i++)
        {
            auto    pAIMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType,this,m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));
            if (nullptr == pAIMesh)
                return E_FAIL;

            m_Meshes.push_back(pAIMesh);
        }

        return S_OK;
    }

    HRESULT CModel::Ready_BinaryMeshes(string binPath, uint32_t ModelType)
    {
        std::ifstream fin(binPath, std::ios::in | std::ios::binary);

        if (fin.is_open()) {
            // 1. 전체 메쉬 개수 읽기

            fin.read((char*)&m_iNumMeshes, sizeof(uint32_t));

            for (uint32_t i = 0; i < m_iNumMeshes; ++i) {
                // 2. 이 메쉬의 정점 개수 읽기
                uint32_t vertexCount = 0;
                fin.read((char*)&vertexCount, sizeof(uint32_t));
                // 3. 정점 배열 읽기
                // 읽어올 크기만큼 벡터 메모리를 미리 확보합니다.
                if (ModelType == 0) {
                    vector<VTXMESH> vertices(vertexCount);
                    fin.read((char*)vertices.data(), sizeof(VTXMESH) * vertexCount);


                    // 4. 인덱스 개수와 배열 읽기
                    uint32_t indexCount = 0;
                    fin.read((char*)&indexCount, sizeof(uint32_t));

                    vector<uint32_t> indices(indexCount);
                    fin.read((char*)indices.data(), sizeof(uint32_t) * indexCount);

                    uint32_t matIndex = 0;
                    fin.read((char*)&matIndex, sizeof(uint32_t));

                    // 5. 읽어온 데이터를 사용하여 CMesh 객체 생성
                    // 이전에 만든 Create_Binary 혹은 전용 초기화 함수를 호출합니다.
                    shared_ptr<CMesh> pMesh = CMesh::Create_Binary(m_pDevice, m_pContext, vertices, indices, matIndex);

                    if (pMesh != nullptr) {
                        m_Meshes.push_back(pMesh);
                    }
                }
                else if (ModelType == 1) {
                    vector<VTXANIMMESH> vertices(vertexCount);
                    fin.read((char*)vertices.data(), sizeof(VTXANIMMESH) * vertexCount);


                    // 4. 인덱스 개수와 배열 읽기
                    uint32_t indexCount = 0;
                    fin.read((char*)&indexCount, sizeof(uint32_t));

                    vector<uint32_t> indices(indexCount);
                    fin.read((char*)indices.data(), sizeof(uint32_t) * indexCount);

                    uint32_t matIndex = 0;
                    fin.read((char*)&matIndex, sizeof(uint32_t));

                    // 5. 읽어온 데이터를 사용하여 CMesh 객체 생성
                    // 이전에 만든 Create_Binary 혹은 전용 초기화 함수를 호출합니다.
                    shared_ptr<CMesh> pMesh = CMesh::Create_Binary(m_pDevice, m_pContext, vertices, indices, matIndex);

                    if (pMesh != nullptr) {
                        m_Meshes.push_back(pMesh);
                    }
                }
            }

            fin.read((char*)&m_iNumMaterials, sizeof(uint32_t));

            for (uint32_t i = 0; i < m_iNumMaterials; i++) {
                vector<string> adTexturePaths[AI_TEXTURE_TYPE_MAX];
                for (uint32_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j) {
                    uint32_t iTextureCount = 0;
                    fin.read((char*)&iTextureCount, sizeof(uint32_t));

                    for (uint32_t k = 0; k < iTextureCount; k++) {
                        uint32_t iLen = 0;
                        fin.read((char*)&iLen, sizeof(uint32_t));

                        string strPath;
                        strPath.resize(iLen);
                        fin.read(&strPath[0], iLen);
                        adTexturePaths[j].push_back(strPath);
                    }
                }
                auto pMaterial = CMaterial::Create_Binary(m_pDevice, m_pContext, adTexturePaths);
                if (pMaterial) m_Materials.push_back(pMaterial);
            }
            fin.close();
        }

        return S_OK;
    }

    HRESULT CModel::Ready_Materials(const _string& strModelFilePath)
    {
        m_iNumMaterials = m_pAIScene->mNumMaterials;

        for (size_t i = 0; i < m_iNumMaterials; i++)
        {
            auto  pMaterial = CMaterial::Create(m_pDevice, m_pContext, m_pAIScene->mMaterials[i], strModelFilePath);

            m_Materials.push_back(pMaterial);
        }

        return S_OK;
    }

    HRESULT CModel::Ready_Bones(const aiNode* pAINode, int32_t iParentBoneIndex)
    {
        auto    pBone = CBone::Create(pAINode, iParentBoneIndex);
        if (nullptr == pBone)
            return E_FAIL;

        m_Bones.push_back(pBone);

        int32_t iParentIndex = m_Bones.size() - 1;

        for (uint32_t i = 0; i < pAINode->mNumChildren; ++i)
        {
            Ready_Bones(pAINode->mChildren[i], iParentIndex);
        }

        return S_OK;
    }

    void CModel::Update_Box(uint32_t modelType)
    {
        Calculate_Box(modelType);
    }

    void CModel::Calculate_Box(uint32_t modelType)
    {
        if (modelType == 0) {
            if (m_Meshes.empty() || m_Meshes[0]->nonAnim_vertices.empty())
                return;

            _float3 firstPos = m_Meshes[0]->nonAnim_vertices[0].vPosition;
            _float minX = firstPos.x, minY = firstPos.y, minZ = firstPos.z;
            _float maxX = firstPos.x, maxY = firstPos.y, maxZ = firstPos.z;

            for (auto& mesh : m_Meshes) {
                for (auto& vertex : mesh->nonAnim_vertices) {
                    if (vertex.vPosition.x < minX) {
                        minX = vertex.vPosition.x;
                    }
                    if (vertex.vPosition.y < minY) {
                        minY = vertex.vPosition.y;
                    }
                    if (vertex.vPosition.z < minZ) {
                        minZ = vertex.vPosition.z;
                    }
                    if (vertex.vPosition.x > maxX) {
                        maxX = vertex.vPosition.x;
                    }
                    if (vertex.vPosition.y > maxY) {
                        maxY = vertex.vPosition.y;
                    }
                    if (vertex.vPosition.z > maxZ) {
                        maxZ = vertex.vPosition.z;
                    }
                }
            }
            max = { maxX, maxY, maxZ };
            min = { minX, minY, minZ };
        }
        else if (modelType == 1) {
            if (m_Meshes.empty() || m_Meshes[0]->Anim_vertices.empty())
                return;

            _float3 firstPos = m_Meshes[0]->Anim_vertices[0].vPosition;
            _float minX = firstPos.x, minY = firstPos.y, minZ = firstPos.z;
            _float maxX = firstPos.x, maxY = firstPos.y, maxZ = firstPos.z;

            for (auto& mesh : m_Meshes) {
                for (auto& vertex : mesh->Anim_vertices) {
                    if (vertex.vPosition.x < minX) {
                        minX = vertex.vPosition.x;
                    }
                    if (vertex.vPosition.y < minY) {
                        minY = vertex.vPosition.y;
                    }
                    if (vertex.vPosition.z < minZ) {
                        minZ = vertex.vPosition.z;
                    }
                    if (vertex.vPosition.x > maxX) {
                        maxX = vertex.vPosition.x;
                    }
                    if (vertex.vPosition.y > maxY) {
                        maxY = vertex.vPosition.y;
                    }
                    if (vertex.vPosition.z > maxZ) {
                        maxZ = vertex.vPosition.z;
                    }
                }
            }
            max = { maxX, maxY, maxZ };
            min = { minX, minY, minZ };
        }
    }
    unique_ptr<CModel> CModel::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, uint32_t eModelType, const _string& strModelFilePath, _fmatrix PreTransformMatrix)
    {
        auto		pInstance = unique_ptr<CModel>(new CModel(pDevice, pContext));

        if (FAILED(pInstance->Initialize_Prototype(eModelType, strModelFilePath,PreTransformMatrix)))
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
