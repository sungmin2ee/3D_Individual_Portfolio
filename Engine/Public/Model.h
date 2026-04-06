#pragma once

#include "Component.h"
#include "Mesh.h"


NS_BEGIN(Engine)
class ENGINE_DLL Model: public CComponent
{
private:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, const _wstring& strPrototypeTag);
public:
	~Model();


private:
	ComPtr<ID3D11Device>					m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		        m_pContext = { nullptr };
	vector<class Mesh>						m_vMeshes;
	const _wstring							prototypeTag = L"";
public:
	void Set_Meshes(vector<Mesh> meshes) { m_vMeshes = meshes; }
	static unique_ptr<Model> Create(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> context, const _wstring& strPrototypeTag);
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual shared_ptr<CPrototype> Clone(void* pArg);
	void Draw();
};

NS_END
