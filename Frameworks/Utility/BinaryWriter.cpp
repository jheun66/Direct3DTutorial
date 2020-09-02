#include "Framework.h"

BinaryWriter::BinaryWriter(wstring filePath)
{
	// 있으면 덮어쓰기형식
	file = CreateFile(filePath.c_str(), GENERIC_WRITE, 0,
		0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

}

BinaryWriter::~BinaryWriter()
{
	CloseHandle(file);
}

void BinaryWriter::Int(int data)
{
	WriteFile(file, &data, sizeof(int), &size, nullptr);
}

void BinaryWriter::UInt(UINT data)
{
	WriteFile(file, &data, sizeof(UINT), &size, nullptr);
}

void BinaryWriter::Float(float data)
{
	WriteFile(file, &data, sizeof(float), &size, nullptr);
}

void BinaryWriter::String(string data)
{
	UInt(data.size());

	const char* str = data.c_str();
	WriteFile(file, str, sizeof(char) * data.size(), &size, nullptr);
}

void BinaryWriter::Float4x4(XMFLOAT4X4 data)
{
	WriteFile(file, &data, sizeof(XMFLOAT4X4), &size, nullptr);
}

void BinaryWriter::Byte(void* data, UINT dataSize)
{
	WriteFile(file, data, dataSize, &size, nullptr);
}
