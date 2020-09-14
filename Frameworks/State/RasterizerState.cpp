#include "Framework.h"

RasterizerState::RasterizerState()
	:desc{}, state(nullptr)
{
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_SOLID;

	Changed();
}

RasterizerState::~RasterizerState()
{
	state->Release();
}


void RasterizerState::SetState()
{
	DC->RSSetState(state);
}

void RasterizerState::FillMode(D3D11_FILL_MODE value)
{
	desc.FillMode = value;

	Changed();
}

void RasterizerState::FrontCounterClockWise(bool value)
{
	desc.FrontCounterClockwise = value;

	Changed();
}

void RasterizerState::Changed()
{
	if (state != nullptr)
		state->Release();


	V(DEVICE->CreateRasterizerState(&desc, &state));
}
