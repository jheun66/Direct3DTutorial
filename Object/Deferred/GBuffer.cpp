#include "Framework.h"

GBuffer::GBuffer()
{
	diffuseRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	specularRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	emissiveRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	normalRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	//normalRTV = new RenderTarget();
	tangentRTV = new RenderTarget();
	depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

	rtvs[0] = diffuseRTV;
	rtvs[1] = normalRTV;
	rtvs[2] = specularRTV;

	srvs[0] = depthStencil->GetSRV();
	srvs[1] = diffuseRTV->GetSRV();
	srvs[2] = normalRTV->GetSRV();
	srvs[3] = specularRTV->GetSRV();
	/*
	rtvs[0] = diffuseRTV;
	rtvs[1] = specularRTV;
	rtvs[2] = emissiveRTV;
	rtvs[3] = normalRTV;
	rtvs[4] = tangentRTV;

	srvs[0] = diffuseRTV->GetSRV();
	srvs[1] = specularRTV->GetSRV();
	srvs[2] = emissiveRTV->GetSRV();
	srvs[3] = normalRTV->GetSRV();
	srvs[4] = tangentRTV->GetSRV();
	*/

	for (UINT i = 0; i < 4; i++)
	{
		targetTexture[i] = new Render2D(L"UV");
		targetTexture[i]->position = { 100 + (float)i * 200, 100, 0 };
		targetTexture[i]->scale = { 200, 200, 200 };
	}

	targetTexture[0]->SetSRV(depthStencil->GetSRV());
	targetTexture[1]->SetSRV(diffuseRTV->GetSRV());
	targetTexture[2]->SetSRV(normalRTV->GetSRV());
	targetTexture[3]->SetSRV(specularRTV->GetSRV());
	/*
	targetTexture[0]->SetSRV(diffuseRTV->GetSRV());
	targetTexture[1]->SetSRV(specularRTV->GetSRV());
	targetTexture[2]->SetSRV(emissiveRTV->GetSRV());
	targetTexture[3]->SetSRV(normalRTV->GetSRV());
	targetTexture[4]->SetSRV(tangentRTV->GetSRV());
	*/
}

GBuffer::~GBuffer()
{
	delete diffuseRTV;
	delete specularRTV;
	delete emissiveRTV;
	delete normalRTV;
	delete tangentRTV;
	delete depthStencil;

	for (Render2D* texture : targetTexture)
		delete texture;
}

void GBuffer::PreRender()
{
	RenderTarget::Sets(rtvs, 3, depthStencil);
}

void GBuffer::Render()
{
	//DC->PSSetShaderResources(10, 4, srvs);
	DC->PSSetShaderResources(3, 1, &srvs[0]);
	DC->PSSetShaderResources(4, 1, &srvs[1]);
	DC->PSSetShaderResources(5, 1, &srvs[2]);
	DC->PSSetShaderResources(6, 1, &srvs[3]);
}

void GBuffer::PostRender()
{
	for (Render2D* texture : targetTexture)
	{
		texture->Update();
		texture->Render();
	}
}
