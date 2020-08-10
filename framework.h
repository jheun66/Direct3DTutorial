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

using namespace DirectX;
using namespace std;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;

// Framework Header
#include "Frameworks/Device/Device.h"

// Object Header


// Program Header


