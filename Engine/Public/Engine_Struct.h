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

	typedef struct tagModelObjectDesc
	{
		uint32_t levelIndex = 0;
		string filePath = "";
		_wstring pModelPrototypeTag = L""; // "Prototype_Component_Model_Desk"
		_wstring pShaderPrototypeTag= L""; // "Prototype_Component_Shader_NonAnim"
		_float4x4  worldMatrix = {};
		_matrix  pretransformMatrix = {};
		_bool   collide = false;
		uint32_t   modelType = {};
	} MODELOBJ_DESC;

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

	//bone indexes which will influence this vertex
	//int32_t boneIDs[MAX_BONE_INFLUENCE] = { -1,-1,-1,-1 };
	////weights from each bone
	//_float weights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//void AddBoneData(uint32_t id, _float weight) {
	//	if (weight <= 0.0f) return;
	//	for (uint32_t i = 0; i < 4; ++i) {
	//		if (weights[i] == 0.0f) { // 빈 슬롯 발견
	//			boneIDs[i] = id;
	//			weights[i] = weight;

	//			float total = 0.0f;
	//			for (int j = 0; j < 4; ++j)
	//				total += weights[j];

	//			// 합이 1.0이 되도록 모든 슬롯을 다시 나눕니다.
	//			if (total > 0.0f) {
	//				for (int j = 0; j < 4; ++j)
	//					weights[j] /= total;
	//			}
	//			return;
	//		}
	//	}

	//	// 만약 4개 이상의 뼈가 영향을 준다면, 보통 가장 작은 가중치를 버리거나 무시합니다.
	//}
	typedef struct VERTEX {
	
		XMFLOAT3 position;
		XMFLOAT3  Normal;
		XMFLOAT2  TexCoords;
		XMFLOAT3  Tangent;

	
		static constexpr uint32_t		iNumElements = { 4 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

	}VERTEX;


	typedef struct VTX_DEBUG {
		XMFLOAT3 vPos;
		XMFLOAT4 vColor;

		static constexpr uint32_t		iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
		  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VCOLLIDER;

	struct Texture {
		std::string type;
		std::string path;
		ComPtr<ID3D11ShaderResourceView> texture;

	};
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vTexcoord;

		static constexpr uint32_t		iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXTEX;

	typedef struct tagVertexPositionNormalTexcoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT2	vTexcoord;

		static constexpr uint32_t		iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXNORTEX;
	
		typedef struct tagVertexMesh
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT3	vTangent;
		XMFLOAT3	vBinormal;
		XMFLOAT2	vTexcoord;


		static constexpr uint32_t		iNumElements = { 5 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXMESH;


		/* 애니메이션이 있는 메시용 정점. */
		typedef struct tagVertexAnimMesh
		{
			XMFLOAT3	vPosition;
			XMFLOAT3	vNormal;
			XMFLOAT3	vTangent;
			XMFLOAT3	vBinormal;
			XMFLOAT2	vTexcoord;

			XMUINT4		vBlendIndices;
			XMFLOAT4	vBlendWeights;


			static constexpr uint32_t		iNumElements = { 7 };
			static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
		}VTXANIMMESH;
}


#endif // Engine_Struct_h__
