#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND		hWnd;
		WINMODE		eWinMode;
		int32_t		iWinSizeX, iWinSizeY;
		int32_t		iNumLevels;
	}ENGINE_DESC;

	struct BoneDesc {
		XMMATRIX BoneMatrices[256];
	};

	struct MatrixBuffer {
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
		BoneDesc bones;
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
		_float weights[MAX_BONE_INFLUENCE]= { 0.0f, 0.0f, 0.0f, 0.0f };

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

	struct asBone // as가 붙은 건 우리만의 버전이다. 
	{
		string name;
		int32_t index = -1;	// 몇 번째 뼈대인지 
		int32_t parent = -1;	// 부모님은 몇번인지 파싱을 하는 도중에 번호를 지어서 관리 
		XMMATRIX transform;	// 유니티에서 봤던 것처럼 계층구조 안에서 SRT 정보가 만들어진다. 그걸 여기에 들고 있는거 
	};

	struct asMesh
	{
		string name;
		aiMesh* mesh;
		vector<VERTEX> vertices;
		vector<uint32_t> indices;
		// 여기까지 default

		int32_t boneIndex; // 매핑할 때 계층구조에서 누구랑 연관 있는지, 어떤 뼈대에 붙어 있는지 추적하기 위한 용도
		string materialName;
	};

	struct asMaterial
	{
		string name;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		string diffuseFile; // 이미지 파일 경로
		string specularFile;
		string normalFile;
	};
}

#endif // Engine_Struct_h__
