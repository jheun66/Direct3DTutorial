#include "Framework.h"

wstring Utility::GetExtension(wstring path)
{
    // ������ . ��ġ ã��
    size_t index = path.find_last_of('.');

    return path.substr(index + 1, path.length());
}
