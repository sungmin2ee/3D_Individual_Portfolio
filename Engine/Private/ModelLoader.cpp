#include "ModelLoader.h"
#include "Model.h"
ModelLoader::ModelLoader(HWND hwnd, ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext) :
	m_pDevice(pDevice),
	m_pContext(pContext),
	meshes_(),
	directory_(),
	textures_loaded_(),
	hwnd_(hwnd) {
	// empty
}


ModelLoader::~ModelLoader() {
	// empty
}

bool ModelLoader::Load(string filename) {
	this->Close();
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace | //탄젠트/비단젠트 계산 (노말용)
		aiProcess_Triangulate | // 모든 폴리곤을 삼각형으로 변환
		aiProcess_JoinIdenticalVertices | // 같은 정점을 합침
		aiProcess_SortByPType |   //점 선 삼각형 타입별로 정리
		aiProcess_GenUVCoords | //UV 좌표 생성
		aiProcess_GenNormals | // 법선 생성
		aiProcess_ConvertToLeftHanded |
		aiProcess_LimitBoneWeights | aiProcess_PopulateArmatureData | aiProcess_PreTransformVertices
	);

	if (pScene == nullptr)
		return false;
	if (!pScene->HasMeshes()) {
		MSG_BOX("이 FBX 파일에는 메쉬 데이터가 정의되어 있지 않습니다.");
		return false;
	}
	this->directory_ = filename.substr(0, filename.find_last_of("/\\"));

	processNode(pScene->mRootNode, pScene);
	return true;
}

//void ModelLoader::Draw() {
//	for (size_t i = 0; i < meshes_.size(); ++i) {
//		meshes_[i].Draw();
//	}
//}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
	// Data to fill
	std::vector<VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		VERTEX vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals()) {
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = (float)mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.TexCoords = { 0.0f, 0.0f };
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	//ProcessBones(mesh, vertices); // 뼈 가중치 처리

	return Mesh(m_pDevice,m_pContext, vertices, indices, textures);
}

vector<Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
	std::vector<Texture> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures_loaded_.size(); j++) {
			if (std::strcmp(textures_loaded_[j].path.c_str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded_[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) {
			HRESULT hr;
			Texture texture;
			ComPtr<ID3D11Resource> dummy;

			// 1. FBX에 저장된 지저분한 경로에서 파일명만 추출
			std::string fullPath = std::string(str.C_Str());
			size_t lastSlash = fullPath.find_last_of("/\\");
			std::string fileNameOnly = (lastSlash != std::string::npos) ? fullPath.substr(lastSlash + 1) : fullPath;

			// 2. 내 프로젝트의 리소스 폴더 경로로 강제 재설정
			std::string myTexturePath = "../../Resources/Textures/" + fileNameOnly;

			// 3. 와이드 스트링 변환 (안전한 방식 권장)
			std::wstring filenamews(myTexturePath.begin(), myTexturePath.end());


			// 4. 내 경로에서 먼저 로드 시도
			hr = CreateWICTextureFromFile(m_pDevice.Get(), m_pContext.Get(), filenamews.c_str(), dummy, texture.texture);

			// 5. 만약 내 폴더에 파일이 없다면, 그때서야 FBX 내부(Embedded) 데이터를 확인
			if (FAILED(hr)) {
				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
				if (embeddedTexture != nullptr) {
					texture.texture = loadEmbeddedTexture(embeddedTexture);
				}
				else {
					// 둘 다 실패했을 때 에러 메시지
					std::string errorMsg = "Texture Not Found in Folder or FBX: " + fileNameOnly;
					MessageBoxA(hwnd_, errorMsg.c_str(), "Error", MB_OK);
				}
			}

			texture.type = typeName;
			texture.path = myTexturePath; // 이제 내 경로가 저장됨
			textures.push_back(texture);
			this->textures_loaded_.push_back(texture);
		}
	}
	return textures;
}

void ModelLoader::Close() {
	textures_loaded_.clear();
	meshes_.clear();
	directory_ = "";
	//for (auto& t : textures_loaded_)
	//	t.Release();

}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {


	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(this->processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

ComPtr<ID3D11ShaderResourceView> ModelLoader::loadEmbeddedTexture(const aiTexture* embeddedTexture) {
	HRESULT hr;
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ComPtr<ID3D11Texture2D> texture2D = nullptr;
		hr = m_pDevice->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
			MSG_BOX("CreateTexture2D failed!");

		hr = m_pDevice->CreateShaderResourceView(texture2D.Get(), nullptr, &texture);
		if (FAILED(hr))
			MSG_BOX("CreateShaderResourceView failed!");

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;
	ComPtr<ID3D11Resource> dummy;

	hr = CreateWICTextureFromMemory(m_pDevice.Get(), m_pContext.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, dummy, texture);
	if (FAILED(hr))
		MSG_BOX("Texture couldn't be created from memory!");
	return texture;
}

unique_ptr<ModelLoader> ModelLoader::Create(HWND hwnd, ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	return unique_ptr<ModelLoader>(new ModelLoader(hwnd,pDevice,pContext));
}
