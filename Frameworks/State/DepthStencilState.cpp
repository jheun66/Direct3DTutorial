#include "Framework.h"
#include "DepthStencilState.h"

DepthStencilState::DepthStencilState()
	: desc{}, state(nullptr)
{
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;


	desc.StencilEnable = true;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;

	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	Changed();
}

DepthStencilState::~DepthStencilState()
{
	state->Release();
}

void DepthStencilState::SetState(UINT stencilRef)
{
	DC->OMSetDepthStencilState(state, stencilRef);
}

void DepthStencilState::DepthEnable(bool value)
{
	desc.DepthEnable = value;
	Changed();
}

void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK value)
{
	desc.DepthWriteMask = value;
	Changed();
}

void DepthStencilState::DepthFunc(D3D11_COMPARISON_FUNC value)
{
	desc.DepthFunc = value;
	Changed();
}

void DepthStencilState::Changed()
{
	if (state != nullptr)
		state->Release();

	V(DEVICE->CreateDepthStencilState(&desc, &state));
}
