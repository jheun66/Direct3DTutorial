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

void BinaryReader::Byte(void** data, UINT dataSize)
{
    ReadFile(file, *data, dataSize, &size, nullptr);
}
