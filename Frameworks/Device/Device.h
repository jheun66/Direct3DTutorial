#pragma once

class Device 
{
private:

	// direct�� �ϵ忡 ���� ������ ó���ϴµ� device�� cpu, context�� gpu�� ������ ó���Ѵ�.
	ID3D11Device* device;                       // CPU, ���ҽ� �Ҵ�
	ID3D11DeviceContext* deviceContext;         // GPU, ������

												// View�� ������ GPU ����
	IDXGISwapChain* swapChain;                  // ���� ���� ����
	ID3D11RenderTargetView* renderTargetView;   // �� ����
	ID3D11DepthStencilView* depthStencilView;

	static Device* instance;

	Device();
	~Device();


public:
	static Device* Get() { return instance; }
	static void Create() { instance = new Device(); }
	static void Delete() { delete instance; }

	void CreateDevcieAndSwapChain();
	void CreateBackBuffer();

	void SetRenderTarget(ID3D11RenderTargetView* rtv = nullptr,
		ID3D11DepthStencilView* dsv = nullptr);

	void Clear(Float4 color = Float4(0.0f, 0.125f, 0.3f, 1.0f), 
		ID3D11RenderTargetView* rtv = nullptr,
		ID3D11DepthStencilView* dsv = nullptr);

	void Present();

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

};