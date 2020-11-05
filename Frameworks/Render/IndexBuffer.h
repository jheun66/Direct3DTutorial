#pragma once

class IndexBuffer
{
private:
	ID3D11Buffer* buffer;

public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	void IASet();
	void Update(void* data, UINT count);
};