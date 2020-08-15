#include "Framework.h"

wstring Utility::GetExtension(wstring path)
{
    // 마지막 . 위치 찾기
    size_t index = path.find_last_of('.');

    return path.substr(index + 1, path.length());
}
