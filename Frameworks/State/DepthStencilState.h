#pragma once

class DepthStencilState
{
private:
	D3D11_DEPTH_STENCIL_DESC desc;
	ID3D11DepthStencilState* state;

public:
	DepthStencilState();
	~DepthStencilState();

	void SetState(UINT stencilRef = 1);

	void DepthEnable(bool value);

	void Changed();
};