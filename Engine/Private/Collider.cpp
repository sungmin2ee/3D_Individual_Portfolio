#include "Collider.h"

CCollider::CCollider(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext) :CComponent(pDevice, pContext)
{
}

CCollider::~CCollider()
{
}
