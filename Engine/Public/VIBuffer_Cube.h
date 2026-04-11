
#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Cube  : public CVIBuffer
{
private:
	VIBuffer_Cube(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

public:
	//VIBuffer_Cube(const VIBuffer_Cube& rhs);
	virtual ~VIBuffer_Cube();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static unique_ptr<VIBuffer_Cube> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
	virtual HRESULT  Render();
	HRESULT Bind_ConstantBuffer(const CB_MATRIX& cbData);
	void UpdateConstantBuffer(const CB_MATRIX& data);
public:
	uint16_t* pIndices;
	VOBB* pVertices;
private:
	ComPtr<ID3D11VertexShader>						m_pVS = { nullptr };
	ComPtr<ID3D11PixelShader>						m_pPS = { nullptr };
	ComPtr<ID3D11InputLayout>						m_pLayout = { nullptr };
	ComPtr<ID3D11SamplerState>						m_pSamplerState = { nullptr };
	ComPtr<ID3D11Buffer>							m_pConstantBuffer = { nullptr };
	_float4x4										m_WorldMatrix = {};

};

NS_END