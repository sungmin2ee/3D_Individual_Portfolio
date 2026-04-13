
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


};

NS_END