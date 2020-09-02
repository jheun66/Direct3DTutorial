#include "Framework.h"

BinaryReader::BinaryReader(wstring filePath)
{
    file = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryReader::~BinaryReader()
{
    CloseHandle(file);
}

int BinaryReader::Int()
{
    int temp;
    ReadFile(file, &temp, sizeof(int), &size, nullptr);
    return temp;
}

UINT BinaryReader::UInt()
{
    UINT temp;
    ReadFile(file, &temp, sizeof(UINT), &size, nullptr);
    return temp;
}

float BinaryReader::Float()
{
    float temp;
    ReadFile(file, &temp, sizeof(float), &size, nullptr);
    return temp;
}

string BinaryReader::String()
{
    UINT size = UInt();

    char* temp = new char[size + 1];
    ReadFile(file, temp, sizeof(char) * size, &this->size, nullptr);
    temp[size] = '\0';

    return temp;
}

XMFLOAT4X4 BinaryReader::Float4x4()
{
    XMFLOAT4X4 matrix;
    matrix._11 = Float();
    matrix._12 = Float();
    matrix._13 = Float();
    matrix._14 = Float();
    matrix._21 = Float();
    matrix._22 = Float();
    matrix._23 = Float();
    matrix._24 = Float();
    matrix._31 = Float();
    matrix._32 = Float();
    matrix._33 = Float();
    matrix._34 = Float();
    matrix._41 = Float();
    matrix._42 = Float();
    matrix._43 = Float();
    matrix._44 = Float();
    return matrix;
}

void BinaryReader::Byte(void** data, UINT dataSize)
{
    ReadFile(file, *data, dataSize, &size, nullptr);
}
