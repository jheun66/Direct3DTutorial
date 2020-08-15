// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN            

// 화면 크기
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// 윈도우 왼쪽 위 위치
#define WIN_START_X 100
#define WIN_START_Y 100

// 디버그 아니면 무시
#ifdef NDEBUG
	#define V(hr) hr
#else
	#define V(hr) assert(SUCCEEDED(hr))
#endif

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#include <windows.h>

#include <assert.h>
#include <vector>
#include <map>
#include <string>

// direct 라이브러리 추가
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

// 프로젝트 속성 - 링커 - 입력 - 추가 종속성에서 추가해 줘도 된다.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
// interface id 있는 라이브러리 
#pragma comment(lib, "dxguid.lib")

// ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// DirectXTex
#include "../DirectXTex/DirectXTex.h"


using namespace DirectX;
using namespace std;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;

// Framework Header
#include "Frameworks/Device/Device.h"

#include "Frameworks/Render/VertexShader.h"
#include "Frameworks/Render/PixelShader.h"
#include "Frameworks/Render/VertexBuffer.h"
#include "Frameworks/Render/IndexBuffer.h"
#include "Frameworks/Render/ConstBuffer.h"
#include "Frameworks/Render/VertexLayouts.h"
#include "Frameworks/Render/GlobalBuffer.h"
#include "Frameworks/Render/Texture.h"

#include "Frameworks/Math/Vector3.h"
#include "Frameworks/Math/Transform.h"

#include "Frameworks/Utility/Utility.h"

#include "Frameworks/State/SamplerState.h"
#include "Frameworks/State/RasterizerState.h"

#include "Enviroment/Enviroment.h"

// Object Header
#include "Object/Quad.h"
#include "Object/Cube.h"

// Program Header
#include "Scene/Scene.h"
#include "Program/Program.h"

extern HWND hWnd;
