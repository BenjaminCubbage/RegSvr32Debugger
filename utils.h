#pragma once
#include <windows.h>
#include <string>
#include <stringapiset.h>
#include <vector>

namespace StringUtils 
{
    std::wstring AnsiToW(LPCSTR s);
    std::wstring AnsiToW(const std::string& s);
    std::string BytesToStringA(const BYTE* bytes, int byteCount = -1);
    std::wstring BytesToStringW(const BYTE* bytes, int byteCount = -1);

    std::wstring TryAnsiToW(LPCSTR s);
    std::string TryBytesToStringA(const BYTE* bytes, int byteCount);
    std::wstring TryBytesToStringW(const BYTE* bytes, int byteCount);

    std::wstring TryRawToW(LPCWSTR s);
}