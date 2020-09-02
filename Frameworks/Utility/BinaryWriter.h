#pragma once

// ������ ������� �о��־���Ѵ�.
class BinaryWriter 
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryWriter(wstring filePath);
	~BinaryWriter();

	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void String(string data);
	void Float4x4(XMFLOAT4X4 data);

	void Byte(void* data, UINT dataSize);
};