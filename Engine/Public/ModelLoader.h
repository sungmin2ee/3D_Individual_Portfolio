#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <vector>
#include <d3d11_1.h>
#include <DirectXMath.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "TextureLoader.h"

using namespace DirectX;

class ModelLoader
{
private:
	ModelLoader(HWND hwnd, ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	~ModelLoader();

	bool Load(string filename);
	void Draw();

	//void Close();
private:
	ComPtr<ID3D11Device>						m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>					m_pContext = { nullptr };
	vector<Mesh> meshes_;
	string directory_;
	vector<Texture> textures_loaded_;
	HWND hwnd_;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ComPtr<ID3D11ShaderResourceView> loadEmbeddedTexture(const aiTexture* embeddedTexture);

public:
	//static shared_ptr<ModelLoader> Create(HWND hwnd, ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

};

#endif // !MODEL_LOADER_H