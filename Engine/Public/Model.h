#pragma once

#include "Component.h"
#include "Mesh.h"
#include "Shader.h"


NS_BEGIN(Engine)
class ENGINE_DLL Model: public CComponent
{
private:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, const string& filename);
public:
	~Model();


private:
	ComPtr<ID3D11Device>					m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		        m_pContext = { nullptr };
	vector<class Mesh>						m_vMeshes;

public:
	void Update_Box();
	void Calculate_Box();
	void Set_Meshes(vector<Mesh> meshes) { m_vMeshes = meshes; }
	static unique_ptr<Model> Create(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> context, const string& filename);
	const _float3 GetMin() { return min; }
	const _float3 GetMax() { return max; }
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual shared_ptr<CPrototype> Clone(void* pArg);
	void Draw();

	void Draw(CShader* pShader);


	_float3 max = { 0.f,0.f,0.f };
	_float3 min = { 0.f,0.f,0.f };
};
  
NS_END
