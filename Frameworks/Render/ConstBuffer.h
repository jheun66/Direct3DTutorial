#pragma once

class ConstBuffer
{
private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;

protected:
	ConstBuffer(void* data, UINT dataSize);
	virtual ~ConstBuffer();

	void MapData();
	
public:
	// shader ���� ����� �����
	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);

};