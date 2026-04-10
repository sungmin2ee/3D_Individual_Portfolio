#include "Helper.h"

CHelper::CHelper(ENGINE_DESC EngineDesc): engineDesc{EngineDesc}
{
}

CHelper::~CHelper()
{
}

void CHelper::GetMousePointRay(_float3* pRayPos, _float3* pRayDir)
{
    // 1. 마우스 스크린 좌표 획득 및 클라이언트 좌표 변환
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(engineDesc.hWnd, &pt);

    // 2. Viewport 공간 -> NDC (Normalized Device Coordinates) 변환
    // x: [-1, 1], y: [1, -1] (DirectX 기준 y축 반전)
    float px = (2.0f * pt.x / engineDesc.iWinSizeX) - 1.0f;
    float py = (-2.0f * pt.y / engineDesc.iWinSizeY) + 1.0f;

    // 3. 투영 행렬 정보를 가져옴
    _float4x4 projf = CGameInstance::Get().GetProj();
    XMMATRIX proj = XMLoadFloat4x4(&projf);

    // 4. View 공간에서의 방향 계산
    // 투영 행렬의 성분을 이용해 NDC를 View 공간으로 되돌림
    // xView = px / Projection(0,0), yView = py / Projection(1,1)
    float xView = px / XMVectorGetX(proj.r[0]);
    float yView = py / XMVectorGetY(proj.r[1]);

    // 5. View 공간의 방향 벡터 생성 (Z=1.0f는 Near Plane 방향)
    XMVECTOR rayDir = XMVectorSet(xView, yView, 1.0f, 0.0f);

    // 6. View 공간 -> World 공간 변환
    _float4x4 viewf = CGameInstance::Get().GetView();
    XMMATRIX view = XMLoadFloat4x4(&viewf);
    XMMATRIX invView = XMMatrixInverse(nullptr, view);

    // 방향 벡터 변환 (위치 이동 제외를 위해 TransformNormal 사용)
    rayDir = XMVector3TransformNormal(rayDir, invView);
    rayDir = XMVector3Normalize(rayDir);

    // 7. Ray Origin은 카메라의 월드 위치
    XMVECTOR rayOrigin = CGameInstance::Get().GetPositionXM();

    // 8. 결과 저장
    XMStoreFloat3(pRayPos, rayOrigin);
    XMStoreFloat3(pRayDir, rayDir);
}
unique_ptr<CHelper> CHelper::Create(ENGINE_DESC EngineDesc)
{
	return unique_ptr<CHelper>(new CHelper(EngineDesc));
}