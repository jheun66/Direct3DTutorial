#include "Framework.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
    : data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    // DEFAULT -> GPU 에서 읽고 쓰기 가능
    // IMMUTABLE -> GPU 에서 읽기만 가능
    // DYNAMIC -> CPU 에서 쓰기가능, GPU 에서 읽기 가능(Map, Unmap 함수 사용 가능)
    // STAGING -> GPU 에서 CPU로 이동(읽기만 가능)
    bd.ByteWidth = dataSize;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    V(DEVICE->CreateBuffer(&bd, nullptr, &buffer));
}

ConstBuffer::~ConstBuffer()
{
    buffer->Release();
}

void ConstBuffer::MapData()
{
    D3D11_MAPPED_SUBRESOURCE subResource;

    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);
}

void ConstBuffer::SetVSBuffer(UINT slot)
{
    MapData();
    DC->VSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetPSBuffer(UINT slot)
{
    MapData();
    DC->PSSetConstantBuffers(slot, 1, &buffer);
}
