#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND		hWnd;
		HINSTANCE   hInst;
		WINMODE		eWinMode;
		int32_t		iWinSizeX, iWinSizeY;
		int32_t		iNumLevels;
	}ENGINE_DESC;

	struct MatrixBuffer { // 슬롯 b0용
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		XMFLOAT4X4 socket;
	};

	struct BoneBuffer { // 슬롯 b1용 (나중에 애니메이션할 때 사용)
		XMMATRIX BoneMatrices[256];
	};
	struct CB_MATRIX
	{
		_float4x4 matWVP;
		_float4 vColor;
	};


	typedef struct VERTEX {
		/*  FLOAT X, Y, Z;
		  XMFLOAT2 texcoord;*/
		XMFLOAT3 position;
		XMFLOAT3  Normal;
		// texCoords
		XMFLOAT2  TexCoords;

		// tangent
		XMFLOAT3  Tangent;

		//bone indexes which will influence this vertex
		int32_t boneIDs[MAX_BONE_INFLUENCE] = { -1,-1,-1,-1 };
		//weights from each bone
		_float weights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f, 0.0f };

		void AddBoneData(uint32_t id, _float weight) {
			if (weight <= 0.0f) return;
			for (uint32_t i = 0; i < 4; ++i) {
				if (weights[i] == 0.0f) { // 빈 슬롯 발견
					boneIDs[i] = id;
					weights[i] = weight;

					float total = 0.0f;
					for (int j = 0; j < 4; ++j)
						total += weights[j];

					// 합이 1.0이 되도록 모든 슬롯을 다시 나눕니다.
					if (total > 0.0f) {
						for (int j = 0; j < 4; ++j)
							weights[j] /= total;
					}
					return;
				}
			}

			// 만약 4개 이상의 뼈가 영향을 준다면, 보통 가장 작은 가중치를 버리거나 무시합니다.
		}

	}VERTEX;


	typedef struct VTX_DEBUG {
		XMFLOAT3 vPos;
		XMFLOAT4 vColor;
	}VOBB;

	struct Texture {
		std::string type;
		std::string path;
		ComPtr<ID3D11ShaderResourceView> texture;

	};
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vTexcoord;
	}VTXTEX;
}


#endif // Engine_Struct_h__
