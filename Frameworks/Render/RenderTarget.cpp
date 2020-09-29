#include "Framework.h"

RenderTarget::RenderTarget(UINT width, UINT height)
{
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		V(DEVICE->CreateTexture2D(&desc, nullptr, &rtvTexture));
	}

	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		V(DEVICE->CreateRenderTargetView(rtvTexture, &desc, &rtv));
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		V(DEVICE->CreateShaderResourceView(rtvTexture, &desc, &srv));
	}
}

RenderTarget::~RenderTarget()
{
	rtvTexture->Release();

	srv->Release();
	rtv->Release();
}

void RenderTarget::Set(DepthStencil* depthStencil)
{
	Device::Get()->SetRenderTarget(rtv, depthStencil->GetDSV());
	Device::Get()->Clear(Float4(0, 0, 0, 0), rtv, depthStencil->GetDSV());
}
