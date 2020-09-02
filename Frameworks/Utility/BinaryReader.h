#pragma once

class BinaryReader
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryReader(wstring filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	string String();
	XMFLOAT4X4 Float4x4();

	void Byte(void** data, UINT dataSize);
};