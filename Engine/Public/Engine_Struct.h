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

	typedef struct tagKeyFrame
	{
		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vTranslation;
		float		fTrackPosition;
	}KEYFRAME;

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
	typedef struct tagVertexInstanceParticle
	{
		XMFLOAT4		vRight, vUp, vLook, vTranslation;
		XMFLOAT2		vLifeTime;
	}VTXINSTANCE_PARTICLE;

	typedef struct tagVertexPosition
	{
		XMFLOAT3	vPosition;


		static constexpr uint32_t		iNumElements = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXPOS;

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
	typedef struct tagVertexCube
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vTexcoord;

		static constexpr uint32_t		iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXCUBE;
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

		typedef struct tagVertexInstanceParticle_Rect
		{
			static constexpr uint32_t		iNumElements = { 7 };
			static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

				{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "TEXCOORD", 5, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			};
		}VTXINSTANCE_PARTICLE_RECT;


		typedef struct tagVertexInstanceParticle_Point
		{
			static constexpr uint32_t		iNumElements = { 6 };
			static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

				{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			};
		}VTXINSTANCE_PARTICLE_POINT;


		typedef struct tagLightDesc
		{
			LIGHT			eType;
			XMFLOAT4		vDiffuse, vAmbient, vSpecular;

			XMFLOAT4		vDirection;
			XMFLOAT4		vPosition;
			float			fRange;
		}LIGHT_DESC;

}	


#endif // Engine_Struct_h__
