#include "Framework.h"

DeferredTarget::DeferredTarget()
{
	diffuseRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	specularRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	emissiveRTV = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	normalRTV = new RenderTarget();
	tangentRTV = new RenderTarget();

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

	depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

	for (UINT i = 0; i < 5; i++)
	{
		targetTexture[i] = new Render2D(L"UV");
		targetTexture[i]->position = { 100 + (float)i * 200,100,0 };
		targetTexture[i]->scale = { 200, 200, 200 };
	}

	targetTexture[0]->SetSRV(diffuseRTV->GetSRV());
	targetTexture[1]->SetSRV(specularRTV->GetSRV());
	targetTexture[2]->SetSRV(emissiveRTV->GetSRV());
	targetTexture[3]->SetSRV(normalRTV->GetSRV());
	targetTexture[4]->SetSRV(tangentRTV->GetSRV());
}

DeferredTarget::~DeferredTarget()
{
	delete diffuseRTV;
	delete specularRTV;
	delete emissiveRTV;
	delete normalRTV;
	delete tangentRTV;
	delete depthStencil;

	for(Render2D* texture : targetTexture)
		delete texture;
}

void DeferredTarget::PreRender()
{
	RenderTarget::Sets(rtvs, 5, depthStencil);
}

void DeferredTarget::Render()
{
	DC->PSSetShaderResources(10, 5, srvs);
}

void DeferredTarget::PostRender()
{
	for (Render2D* texture : targetTexture)
	{
		texture->Update();
		texture->Render();
	}
}
