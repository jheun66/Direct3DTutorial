#pragma once

class VertexBuffer
{
private:
	ID3D11Buffer* buffer;

	UINT stride;
	UINT offset;

public:
	VertexBuffer(void* data, UINT stride, UINT count);
	~VertexBuffer();

	void IASet(UINT slot = 0);

	void Update(void* data, UINT count);

	ID3D11Buffer* GetBuffer() { return buffer; }
};