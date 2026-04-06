
#include "TextureLoader.h"
NS_BEGIN(Engine)

using namespace DirectX;


class ModelLoader
{
private:
	ModelLoader(HWND hwnd, ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	~ModelLoader();

	unique_ptr<class Model> Load(string filename);
	// Draw();

	void Close();
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
	static unique_ptr<ModelLoader> Create(HWND hwnd, ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

};

NS_END
