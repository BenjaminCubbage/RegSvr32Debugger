#include "utils.h"

#include <iostream>

namespace StringUtils 
{
    std::wstring AnsiToW(LPCSTR s)
    {
        if (!s) 
            return std::wstring();
        int needed = MultiByteToWideChar(CP_ACP, 0, s, -1, nullptr, 0);
        
        if (needed <= 0) 
            return std::wstring();
        
        std::vector<wchar_t> buf(needed);
        MultiByteToWideChar(CP_ACP, 0, s, -1, buf.data(), needed);
        
        return std::wstring(buf.data());
    }

    std::wstring AnsiToW(const std::string& s) 
    {
        return AnsiToW(s.c_str());
    }

    std::string BytesToStringA(const BYTE* bytes, int byteCount)
    {
        if (byteCount == -1) 
            return std::string(reinterpret_cast<const char*>(bytes));

        std::string str(reinterpret_cast<const char*>(bytes),
                        reinterpret_cast<const char*>(bytes + byteCount));

        if (!str.empty() && str.back() == '\0')
            str.pop_back();

        return str;
    }

    std::wstring BytesToStringW(const BYTE* bytes, int byteCount)
    {
        if (byteCount == -1) 
            return std::wstring(reinterpret_cast<const wchar_t*>(bytes));

        std::wstring str(reinterpret_cast<const wchar_t*>(bytes),
                         reinterpret_cast<const wchar_t*>(bytes + byteCount));

        if (!str.empty() && str.back() == L'\0')
            str.pop_back();

        return str;
    }

    std::wstring TryAnsiToW(LPCSTR s)
    {
        return s ? AnsiToW(s) : std::wstring();
    }

    std::string TryBytesToStringA(const BYTE* bytes, int byteCount)
    {
        return byteCount <= 0 || bytes == nullptr ? "" : BytesToStringA(bytes, byteCount);
    }

    std::wstring TryBytesToStringW(const BYTE* bytes, int byteCount)
    {
        return byteCount <= 0 || bytes == nullptr ? L"" : BytesToStringW(bytes, byteCount);
    }

    std::wstring TryRawToW(LPCWSTR s)
    {
        return s ? std::wstring(s) : std::wstring();
    }
}