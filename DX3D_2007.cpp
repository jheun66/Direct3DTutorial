// DX3D_2007.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "DX3D_2007.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11InputLayout* inputLayout;
ID3D11Buffer* vertexBuffer;
ID3D11Buffer* indexBuffer;
ID3D11Buffer* constantBuffer;

struct WVP
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
}wvp;

// 정점(Vertex) : 3D 공간에서의 한 점
struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
};


void InitDevice();
void Render();
void ReleaseDevice();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX3D2007, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX3D2007));

    // 구조체 초기화, zeromemory와 동일
    MSG msg = {};

    InitDevice();

    // PeekMessage vs GetMessage : 반환값 차이
    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // 프로시저로 메시지 넘김
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Update();
            Render();
        }
    }

    ReleaseDevice();

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX3D2007));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX3D2007);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // device 크기를 요거로
   RECT rc = { 0,0,WIN_WIDTH,WIN_HEIGHT };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      WIN_START_X, WIN_START_Y,
       rc.right- rc.left, rc.bottom -rc.top,
       nullptr, nullptr, hInstance, nullptr);

   SetMenu(hWnd, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    // 처음 그릴때랑 화면 가렸다가 그릴때
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void InitDevice()
{
    

    ID3D11Texture2D* backBuffer;

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

    // back버퍼는 사용해제
    backBuffer->Release();

    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////

    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &vp);

    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    // shader load 할때 사용(shader마다 따로)
    ID3DBlob* vertexBlob;
    //                                                              VS 진입점 이름, vs_.. 버전
    D3DCompileFromFile(L"Shaders/Tutorial.hlsl", nullptr, nullptr,
        "VS", "vs_5_0", flags, 0, &vertexBlob, nullptr);

    device->CreateVertexShader(vertexBlob->GetBufferPointer(),
        vertexBlob->GetBufferSize(), nullptr, &vertexShader);

    /*
    typedef struct D3D11_INPUT_ELEMENT_DESC
    {
    LPCSTR SemanticName;
    UINT SemanticIndex;
    DXGI_FORMAT Format;
    UINT InputSlot;
    UINT AlignedByteOffset;
    D3D11_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
    } 	D3D11_INPUT_ELEMENT_DESC;
    */
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"CoLor", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
        D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT layoutSize = ARRAYSIZE(layout);

    device->CreateInputLayout(layout, layoutSize,
        vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(),
        &inputLayout);
    vertexBlob->Release();

    ID3DBlob* pixelBlob;

    D3DCompileFromFile(L"Shaders/Tutorial.hlsl", nullptr, nullptr,
        "PS", "ps_5_0", flags, 0, &pixelBlob, nullptr);

    device->CreatePixelShader(pixelBlob->GetBufferPointer(),
        pixelBlob->GetBufferSize(), nullptr, &pixelShader);

    pixelBlob->Release();

    // 폴리곤(Polygon) : 3D 공간에서의 삼각형 (방향 존재 : 시계방향으로 앞면이 결정된다)
    Vertex vertices[8];
    vertices[0].pos = { -1, -1, -1 };
    vertices[1].pos = { -1, 1, -1 };
    vertices[2].pos = { 1, 1, -1 };
    vertices[3].pos = { 1, -1, -1 };

    vertices[4].pos = { -1, -1, 1 };
    vertices[5].pos = { -1, 1, 1 };
    vertices[6].pos = { 1, 1, 1 };
    vertices[7].pos = { 1, -1, 1 };

    vertices[0].color = { 1,0,0,1 };
    vertices[1].color = { 1,1,0,1 };
    vertices[2].color = { 1,0,1,1 };
    vertices[3].color = { 0,1,1,1 };
    vertices[4].color = { 0,1,0,1 };
    vertices[5].color = { 0,0,1,1 };
    vertices[6].color = { 1,1,1,1 };
    vertices[7].color = { 0,0,0,1 };

    {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vertex) * 8;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices;

        device->CreateBuffer(&bd, &initData, &vertexBuffer);
    }

    UINT indices[] =
    {
        // f
        0,1,2,
        0,2,3,

        // u
        1,5,6,
        1,6,2,

        // r
        3,2,6,
        3,6,7,
    };

    {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(indices);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices;

        device->CreateBuffer(&bd, &initData, &indexBuffer);
    }

    {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(WVP);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        device->CreateBuffer(&bd, nullptr, &constantBuffer);
    }

    wvp.world = XMMatrixIdentity();
    XMVECTOR eye = XMVectorSet(3, 3, -3, 0);
    XMVECTOR focus = XMVectorSet(0, 0, 0, 0);
    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    wvp.view = XMMatrixLookAtLH(eye, focus, up);
    wvp.projection = XMMatrixPerspectiveFovLH(XM_PIDIV2,
        width / (float)height, 0.1f, 1000.0f);

}

void Render()
{
    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    // 상수버퍼로 넘겨줄 때
    WVP temp;
    temp.world = XMMatrixTranspose(wvp.world);
    temp.view = XMMatrixTranspose(wvp.view);
    temp.projection = XMMatrixTranspose(wvp.projection);

    deviceContext->UpdateSubresource(constantBuffer, 0, nullptr,
        &temp, 0, 0);

    // Render
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // 세팅
    deviceContext->IASetInputLayout(inputLayout);

    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);

    // 그리기
    deviceContext->DrawIndexed(36, 0, 0);

    swapChain->Present(0, 0);
}

void ReleaseDevice()
{
    constantBuffer->Release();
    indexBuffer->Release();
    vertexBuffer->Release();

    vertexShader->Release();
    pixelShader->Release();
    inputLayout->Release();

    renderTargetView->Release();

    swapChain->Release();
    deviceContext->Release();
    device->Release();
}
