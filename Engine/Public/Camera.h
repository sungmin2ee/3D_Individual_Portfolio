#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)

class  CCamera
{
private:
	CCamera();
public:
	~CCamera();

public:

	XMVECTOR GetPositionXM() const;
	XMFLOAT3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM() const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;
	_float GetNearZ() const;
	_float GetFarZ() const;
	_float GetAspect() const;
	_float GetFovY() const;
	_float GetFovX() const;
	_float GetNearWindowWidth() const;
	_float GetNearWindowHeight() const;
	_float GetFarWindowWidth() const;
	_float GetFarWindowHeight() const;
	// 절 두 체 를 설 정 한 다 .
	void SetLens(float fovY, float aspect, float zn ,float zf);
	// 카 메 라 위치와 시선 및 위쪽 방향으로 카메라 공간을 설정한다.
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target,const XMFLOAT3& up);
	// 시야 행렬과 투영 행렬을 돌려준다.
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;
	//카 메 라 를 거리 d만큼 횡이동, 축이동한다 .
	void Strafe(float d);
	void Walk(float d);
	//?WI 라를 회전한다.
	void Pitch(float angle);
	void RotateY(float angle);
	//매 프 레 임 □[다 , 카 메 라 위 치 나 방 향 을 수 정 한 후 
	// 이 메서드를 호출해서 시0 ^ 행렬을 재 구 축 한 다 .
	void UpdateViewMatrix();
	const XMFLOAT4X4 GetView() { return mView; }
	const XMFLOAT4X4 GetProj() { return mProj; }
private:
	//세 계 공 간 기준의 카메라 좌표계
	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mAt;
	XMFLOAT3 mLook;
	_float mNearZ;
	_float mFarZ;
	_float mAspect;
	_float mFovY;
	_float mNearWindowHeight;
	_float mFarWindowHeight;
	//시 야 행 렬 과 투 영 행렬
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

public:
	static unique_ptr<CCamera> Create();

};

NS_END

