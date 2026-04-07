#include "Camera.h"

CCamera::CCamera()
{
	XMVECTOR Eye = XMVectorSet(0.0f, 5.0f, -100.0f, 1.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	LookAt(Eye, At, Up);

	// mLook= 
	UpdateViewMatrix();
}

CCamera::~CCamera()
{
}

XMVECTOR CCamera::GetPositionXM() const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 CCamera::GetPosition() const
{
	return mPosition;
}

void CCamera::SetPosition(float x, float y, float z)
{
}

void CCamera::SetPosition(const XMFLOAT3& v)
{
}

XMVECTOR CCamera::GetRightXM() const
{
	return XMVECTOR();
}

XMFLOAT3 CCamera::GetRight() const
{
	return XMFLOAT3();
}

XMVECTOR CCamera::GetUpXM() const
{
	return XMVECTOR();
}

XMFLOAT3 CCamera::GetUp() const
{
	return XMFLOAT3();
}

XMVECTOR CCamera::GetLookXM() const
{
	return XMVECTOR();
}

XMFLOAT3 CCamera::GetLook() const
{
	return XMFLOAT3();
}

_float CCamera::GetNearZ() const
{
	return _float();
}

_float CCamera::GetFarZ() const
{
	return _float();
}

_float CCamera::GetAspect() const
{
	return _float();
}

_float CCamera::GetFovY() const
{
	return _float();
}

_float CCamera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

_float CCamera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

_float CCamera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

_float CCamera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

_float CCamera::GetFarWindowHeight() const
{

	return mFarWindowHeight;
}

void CCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;
	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);
	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);

}

void CCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{

	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void CCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{

}

XMMATRIX CCamera::View() const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX CCamera::Proj() const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX CCamera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

void CCamera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void CCamera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

void CCamera::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void CCamera::RotateY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void CCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	// 1. 서로 직교하도록 재정렬 (Gram-Schmidt 과정)
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	// 2. View 행렬의 이동 성분 계산 (내적)
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	// 3. 행렬 채우기 (View 행렬은 카메라 변환의 역행렬임)
	mView(0, 0) = mRight.x; mView(0, 1) = mUp.x; mView(0, 2) = mLook.x; mView(0, 3) = 0.0f;
	mView(1, 0) = mRight.y; mView(1, 1) = mUp.y; mView(1, 2) = mLook.y; mView(1, 3) = 0.0f;
	mView(2, 0) = mRight.z; mView(2, 1) = mUp.z; mView(2, 2) = mLook.z; mView(2, 3) = 0.0f;
	mView(3, 0) = x;        mView(3, 1) = y;      mView(3, 2) = z;       mView(3, 3) = 1.0f;

}

unique_ptr<CCamera> CCamera::Create()
{

	return unique_ptr<CCamera>(new CCamera());
}