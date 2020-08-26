#include "Framework.h"

RawBuffer::RawBuffer(void* inputData, UINT byteWidth)
	:inputData(inputData), byteWidth(byteWidth)
{
	CreateOutput();
	CreateUAV();
	CreateResult();
}

RawBuffer::~RawBuffer()
{
	output->Release();
	uav->Release();
	result->Release();
}

void RawBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	memcpy(data, subResource.pData, size);
	DC->Unmap(result, 0);
}

void RawBuffer::CreateOutput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	V(DEVICE->CreateBuffer(&bd, nullptr, &buffer));

	// 버퍼가 resource 상속받아서 형변환 가능
	output = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC bd = {};
	buffer->GetDesc(&bd);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	// 형상관없이 바이트만 32bit 잡겟다
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = bd.ByteWidth / 4;

	V(DEVICE->CreateUnorderedAccessView(buffer, &uavDesc, &uav));
}

void RawBuffer::CreateResult()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = {};
	((ID3D11Buffer*)output)->GetDesc(&bd);
	bd.Usage = D3D11_USAGE_STAGING;
	bd.BindFlags = 0;
	bd.MiscFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	V(DEVICE->CreateBuffer(&bd, nullptr, &buffer));

	// 버퍼가 resource 상속받아서 형변환 가능
	result = (ID3D11Resource*)buffer;
}
