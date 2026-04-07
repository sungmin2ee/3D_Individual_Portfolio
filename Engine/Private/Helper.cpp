#include "Helper.h"

void CHelper::GetMousePointRay(const ENGINE_DESC& EngineDesc , _float3* pRayPos, _float3* pRayDir)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(EngineDesc.hWnd, &pt);

	float px = (2.0f * pt.x / EngineDesc.iWinSizeX - 1.0f);
	float py = (-2.0f * pt.y / EngineDesc.iWinSizeY + 1.0f);
	XMVECTOR rayDir = XMVectorSet(px, py, 1.0f, 0.0f);
	_float4x4 projf = CGameInstance::Get().GetProj();
	XMMATRIX proj = XMLoadFloat4x4(&projf);
	rayDir = XMVector3TransformNormal(rayDir, XMMatrixInverse(nullptr, proj));
	_float4x4 viewf = CGameInstance::Get().GetView();
	XMMATRIX view = XMLoadFloat4x4(&viewf);
	rayDir = XMVector3TransformNormal(rayDir, XMMatrixInverse(nullptr, view));

	rayDir = XMVector3Normalize(rayDir);

	// Ray Origin = 카메라 위치
	XMVECTOR rayOrigin = CGameInstance::Get().GetPositionXM();

	// 6. 결과를 매개변수(_float3*)에 저장
	XMStoreFloat3(pRayPos, rayOrigin); // 레이 시작점 저장
	XMStoreFloat3(pRayDir, rayDir);    // 레이 방향 저장
}
