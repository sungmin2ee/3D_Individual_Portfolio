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

	struct VERTEX {
		FLOAT X, Y, Z;
		XMFLOAT2 texcoord;
	};

	struct Texture {
		std::string type;
		std::string path;
		ComPtr<ID3D11ShaderResourceView> texture;

	};
}


#endif // Engine_Struct_h__
