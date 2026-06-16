    #include "CModel.h"
    #include "GameInstance.h"
    #include "ThreadPool.h"

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
        , m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
        , m_iNumAnimations{ Prototype.m_iNumAnimations }
    {
        for (auto& pPrototypeBone : Prototype.m_Bones)
        {
            m_Bones.push_back(pPrototypeBone->Clone());
        }

        for (auto& pPrototypeAnim : Prototype.m_Animations)
        {
            m_Animations.push_back(pPrototypeAnim->Clone());
        }
        m_Snapshots.resize(m_Bones.size());
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

    const _float4x4* CModel::Get_BoneMatrixPtr(const _char* pBoneName)
    {
        auto    iter = find_if(m_Bones.begin(), m_Bones.end(), [&](shared_ptr<CBone> pBone)->_bool
            {
                if (true == pBone->Compare_Name(pBoneName))
                    return true;

                return false;
            });

        if (iter == m_Bones.end())
            return nullptr;

        return (*iter)->Get_CombinedTransformationMatrixPtr();
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
            Ready_BinaryModel(binPath, m_eModelType);
            return S_OK;
            //return LoadFromBinary(binPath); // 바이너리 폴더에서 읽기
        }
        else {

           // uint32_t        iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_SortByPType |   //점 선 삼각형 타입별로 정리
           //     aiProcess_LimitBoneWeights | aiProcess_PopulateArmatureData };
            uint32_t        iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

            if (ETOUI(MODEL::NONANIM) == m_eModelType)
                iFlag |= aiProcess_PreTransformVertices;


            m_pAIScene = m_Importer.ReadFile(strModelFilePath, iFlag);
            if (nullptr == m_pAIScene)
                return E_FAIL;

            XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

            if (ETOUI(MODEL::ANIM) == m_eModelType)
                  Ready_Bones(m_pAIScene->mRootNode, -1);

            if (FAILED(Ready_Meshes()))
                return E_FAIL;

            if (FAILED(Ready_Materials(strModelFilePath)))
                return E_FAIL;

            if (ETOUI(MODEL::ANIM) == m_eModelType)
            {
                if (FAILED(Ready_Animation()))
                    return E_FAIL;
            }

            // 메쉬 정보 
            std::ofstream fout;
            fout.open(binPath, std::ios::out | std::ios::binary);

            if (fout.is_open()) {
                fout.write((const char*)&m_PreTransformMatrix, sizeof(_float4x4));
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

                        uint32_t NumBones = mesh->Get_NumBones(); // CMesh에 인덱스 반환 함수가 있다고 가정
                        fout.write((const char*)&NumBones, sizeof(uint32_t));

                        if (NumBones > 0) {
                            // Offset Matrices
                            const char* pName = mesh->Get_BoneName();
                            uint32_t iNameLen = (uint32_t)strlen(pName) + 1; // 널 문자(\0) 포함
                            // 1. 이름의 길이를 먼저 저장
                            fout.write((const char*)&iNameLen, sizeof(uint32_t));
                            // 2. 실제 이름 데이터를 저장
                            fout.write(pName, iNameLen);
                            fout.write((const char*)mesh->Get_OffsetMatrices().data(), sizeof(_float4x4)* NumBones);
                            fout.write((const char*)mesh->Get_BoneIndices().data(), sizeof(uint32_t) * NumBones);
                        }
                    }

           
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


                ////Bone 저장

                uint32_t boneCount = static_cast<uint32_t>(m_Bones.size());
                fout.write((const char*)&boneCount, sizeof(uint32_t));

                for (auto& pBone : m_Bones) {
                    // 1. 이름 저장
                    const char* pName = pBone->Get_BoneName();
                    int32_t iNameLen = (int32_t)strlen(pName) + 1; // 널 문자(\0) 포함
                    fout.write((const char*)&iNameLen, sizeof(int32_t));
                    fout.write(pName, iNameLen);
                    // 2. 부모 인덱스 저장
                    int32_t iParentIndex = pBone->Get_ParentBoneIndex();
                    fout.write((char*)&iParentIndex, sizeof(int32_t));

                    // 3. 변환 행렬들 저장 (Transformation, OffsetMatrix)
                    _float4x4 matTransform = pBone->Get_TransformationMatrix(); // OffsetMatrix
                    fout.write((char*)&matTransform, sizeof(_float4x4));

                }
                // 애니메이션 저장
                fout.write((const char*)&m_iNumAnimations, sizeof(uint32_t));
                for (auto& animation : m_Animations) {

                    const char* animName = animation->Get_Name();
                    int32_t iNameLen = (int32_t)strlen(animName) + 1; // 널 문자(\0) 포함
                    auto duration =animation->Get_Duration();
                    auto numbChannels = animation->Get_NumbChannels();
                    auto tickPerSec = animation->Get_TickPerSec();
                    fout.write((const char*)&iNameLen, sizeof(int32_t));
                    fout.write(animName, iNameLen);
                    fout.write((char*)&duration, sizeof(_float));
                    fout.write((char*)&numbChannels, sizeof(uint32_t));
                    fout.write((char*)&tickPerSec, sizeof(_float));

                    auto channels = animation->Get_Channels();
                    for (uint32_t i = 0; i < channels.size(); i++) {
                        int32_t index = channels[i]->Get_BoneIndex();
                        uint32_t numKeyFrames = channels[i]->Get_NumKeyFrames();
                        const auto& keyFrames = channels[i]->Get_KeyFrames();

                        fout.write((char*)&index, sizeof(int32_t));
                        fout.write((char*)&numKeyFrames, sizeof(uint32_t));
                        fout.write((char*)keyFrames.data(), sizeof(KEYFRAME)* numKeyFrames);

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
    _bool CModel::Play_Animation(_float fTimeDelta)
    {




        fTimeDelta = fTimeDelta * m_fAnimSpeed;
        _bool           isFinished = { false };
        if (m_bAnimChanged)
        {
            m_bIsBlending = true;
            m_fBlendTime = 0.f;

            for (uint32_t i = 0; i < m_Bones.size(); ++i)
                m_Snapshots[i] = m_Bones[i]->Get_Current_SRT();
        }

        if (m_bIsBlending)
        {
            m_fBlendTime += fTimeDelta;
            _float fRatio = m_fBlendTime / 0.2f; // 0.2초 동안 섞음
            if (fRatio >= 1.f)
            {
                m_bIsBlending = false;
                // 보간이 끝났으므로 정상 업데이트
                isFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(fTimeDelta, m_Bones, m_isAnimLoop, m_bAnimChanged);
            }
            else
            {
                // [핵심] 새 애니메이션의 SRT를 구한 뒤 스냅샷과 섞어서 Bone에 셋팅
                // 새 애니메이션도 시간은 흘러야 하므로 fTimeDelta 전달
                 m_Animations[m_iCurrentAnimIndex]->Update_Blending_Matrices(fTimeDelta, m_Bones, m_Snapshots, fRatio, m_bAnimChanged);
            }
        }
        else {
            isFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(fTimeDelta, m_Bones, m_isAnimLoop, m_bAnimChanged);
        }

        /* 뼈들의 m_TransformationMatrix를 갱신해준다. */

      
        for (auto& pBone : m_Bones)
        {
            pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
        }
        m_bAnimChanged = false;

        return isFinished;

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

    HRESULT CModel::Ready_BinaryModel(string binPath, uint32_t ModelType)
    {
   
        std::ifstream fin(binPath, std::ios::in | std::ios::binary);

        if (fin.is_open()) {
            // 1. 전체 메쉬 개수 읽기
            fin.read((char*)&m_PreTransformMatrix, sizeof(_float4x4));
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
                    shared_ptr<CMesh> pMesh = CMesh::Create_Binary(m_pDevice, m_pContext, vertices, indices, matIndex,0);

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

                    uint32_t iNumBones = 0;
                    fin.read((char*)&iNumBones, sizeof(uint32_t));

                    char szBoneName[MAX_PATH] = {};
                    vector<_float4x4> offsetMat;
                    vector<uint32_t> BoneIndices;
                    offsetMat.resize(iNumBones);
                    BoneIndices.resize(iNumBones);

                    if (iNumBones > 0) {
                        int32_t iNameLen = 0;
                        fin.read((char*)&iNameLen, sizeof(uint32_t));
                        fin.read(szBoneName, iNameLen);
                        fin.read((char*)offsetMat.data(), sizeof(_float4x4) * iNumBones);
                        fin.read((char*)BoneIndices.data(), sizeof(uint32_t) * iNumBones);
                    }
 
                    // 5. 읽어온 데이터를 사용하여 CMesh 객체 생성
                    // 이전에 만든 Create_Binary 혹은 전용 초기화 함수를 호출합니다.
                    shared_ptr<CMesh> pMesh = CMesh::Create_Binary(m_pDevice, m_pContext, vertices, indices, matIndex, iNumBones);
                    pMesh->Set_Name(szBoneName);
                    pMesh->Set_OffsetMatrices(offsetMat);
                    pMesh->Set_BoneIndices(BoneIndices);
                    //pMesh->set
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
            uint32_t boneCount = 0;
            fin.read((char*)&boneCount, sizeof(uint32_t));

            for (int32_t i = 0; i < boneCount; i++) {
                char BoneName[MAX_PATH] = {};
                uint32_t boneNameLen = 0;
                fin.read((char*)&boneNameLen, sizeof(uint32_t));
                fin.read(BoneName, boneNameLen);

                uint32_t parentIndex = 0;
                fin.read((char*)&parentIndex, sizeof(uint32_t));
                _float4x4 matTransform;
                fin.read((char*)&matTransform, sizeof(_float4x4));

                auto    pBone = CBone::Create_Binary(BoneName, matTransform,parentIndex);
                m_Bones.push_back(pBone);

            }

            //애니메이션 읽기 

     
            fin.read((char*)&m_iNumAnimations, sizeof(uint32_t));

            for (uint32_t i = 0; i < m_iNumAnimations; i++) {
                _float duration = 0;
                uint32_t numbChannels = 0;
                _float tickPerSec = 0;

                char animName[MAX_PATH] = {};
                uint32_t animNameLen = 0;
                fin.read((char*)&animNameLen, sizeof(uint32_t));
                fin.read(animName, animNameLen);
                fin.read((char*)&duration, sizeof(_float));
                fin.read((char*)&numbChannels, sizeof(uint32_t));
                fin.read((char*)&tickPerSec, sizeof(_float));
                vector<shared_ptr<CChannel>> b_Channels;

                for (uint32_t j = 0; j < numbChannels; j++) {
                    int32_t boneIndex = 0;
                    uint32_t numKeyFrames = 0;
                    fin.read((char*)&boneIndex, sizeof(int32_t));
                    fin.read((char*)&numKeyFrames, sizeof(uint32_t));
                    vector<KEYFRAME> keyframes(numKeyFrames);
                    fin.read((char*)keyframes.data(), sizeof(KEYFRAME)* numKeyFrames);
                    auto channel = CChannel::Create_Binary(boneIndex, numKeyFrames, move(keyframes));
                    b_Channels.push_back(channel);
                }
                auto animation = CAnimation::Create_Binary(animName, duration, tickPerSec, numbChannels, b_Channels);
                m_Animations.push_back(animation);
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

    HRESULT CModel::Ready_Animation()
    {
        m_iNumAnimations = m_pAIScene->mNumAnimations;

        for (size_t i = 0; i < m_iNumAnimations; i++)
        {
            shared_ptr<CAnimation> pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
            if (nullptr == pAnimation)
                return E_FAIL;

            
            m_Animations.push_back(pAnimation);
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
