#include "Framework.h"

StructuredBuffer::StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
	: inputData(inputData), inputStride(inputStride), inputCount(inputCount),
	outputStride(outputStride), outputCount(outputCount)
{
	if (outputStride == 0 || outputCount == 0)
	{
		this->outputStride = inputStride;
		this->outputCount = inputCount;
	}

	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

StructuredBuffer::~StructuredBuffer()
{
	input->Release();
	srv->Release();
	output->Release();
	uav->Release();
	result->Release();
}

void StructuredBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResoruce;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResoruce);
	memcpy(data, subResoruce.pData, size);
	DC->Unmap(result, 0);
}

void StructuredBuffer::CreateInput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = inputStride * inputCount;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = inputStride;

	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = inputData;

	V(DEVICE->CreateBuffer(&bd, &subResource, &buffer));

	input = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateSRV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)input;

	D3D11_BUFFER_DESC bd;
	buffer->GetDesc(&bd);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.NumElements = inputCount;

	V(DEVICE->CreateShaderResourceView(buffer, &srvDesc, &srv));
}

void StructuredBuffer::CreateOutput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = outputStride * outputCount;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = outputStride;

	V(DEVICE->CreateBuffer(&bd, nullptr, &buffer));

	output = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC bd = {};
	buffer->GetDesc(&bd);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = outputCount;

	V(DEVICE->CreateUnorderedAccessView(buffer, &uavDesc, &uav));
}

void StructuredBuffer::CreateResult()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = {};
	((ID3D11Buffer*)output)->GetDesc(&bd);
	bd.Usage = D3D11_USAGE_STAGING;
	bd.BindFlags = 0;
	bd.MiscFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	V(DEVICE->CreateBuffer(&bd, nullptr, &buffer));

	result = (ID3D11Resource*)buffer;
}
