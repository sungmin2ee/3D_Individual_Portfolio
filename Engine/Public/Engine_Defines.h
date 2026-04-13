#pragma once

#include <d3d11.h>

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "Fx11/d3dx11effect.h"
#include <DirectXCollision.h>
using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <memory>
#include <wrl/client.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <nlohmann/json.hpp>
//#ifdef _DEBUG
//#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
//#pragma comment(lib, "FX11/Effects11d.lib")
//#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")
//#else
//#pragma comment(lib, "DirectXTex/DirectXTex.lib")
//#pragma comment(lib, "FX11/Effects11.lib")
//#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
//#endif
using namespace Microsoft::WRL;

using namespace std;

#include "imgui.h"

#include "imgui_internal.h"
#include "ImGuizmo.h"
#include "ImSequencer.h"
#include "ImZoomSlider.h"
#include "ImCurveEdit.h"
#include "GraphEditor.h"

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif


using namespace Engine;


