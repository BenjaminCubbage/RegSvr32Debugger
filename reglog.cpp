#include "reglog.h"
#include "utils.h"
#include <sstream>

RegLog::RegLog()
{
	keys.insert_or_assign(HKEY_CLASSES_ROOT,   L"HKCR");
	keys.insert_or_assign(HKEY_CURRENT_USER,   L"HKCU");
	keys.insert_or_assign(HKEY_LOCAL_MACHINE,  L"HKLM");
	keys.insert_or_assign(HKEY_CURRENT_CONFIG, L"HKCC");
	keys.insert_or_assign(HKEY_USERS,		   L"HKU");
}

std::wstring RegLog::GetQualifiedPath(HKEY hKey)
{
	return keys.count(hKey) ? keys.at(hKey) : L"<UNKNOWN HKEY>";
}

std::wstring RegLog::GetQualifiedPath(HKEY hKey, std::wstring subpath)
{
	std::wstringstream ss;
	ss << GetQualifiedPath(hKey) << L"\\" << subpath;
	return ss.str();
}

void RegLog::Log(std::wstring path, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value, std::wstring valueName)
{
	if (valueName.empty())
		valueName = L"(DEFAULT)";

	eventLog.emplace_back(RegEvent(type, statusCode, path, value, valueName));
}

void RegLog::Log(HKEY hKey, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value, std::wstring valueName)
{
	Log(GetQualifiedPath(hKey), type, statusCode, value, valueName);
}

void RegLog::LogRawAnsi(LPCSTR path, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCSTR valueName)
{
	Log(StringUtils::AnsiToW(path), type, statusCode, StringUtils::AnsiToW(StringUtils::TryBytesToStringA(value, valueBytes)), valueName ? StringUtils::AnsiToW(valueName) : L"");
}

void RegLog::LogRawWide(LPCWSTR path, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCWSTR valueName)
{
	Log(path, type, statusCode, StringUtils::TryBytesToStringW(value, valueBytes), valueName ? valueName : L"");
}

void RegLog::LogRawAnsi(HKEY hKey, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCSTR valueName)
{
	Log(hKey, type, statusCode, StringUtils::AnsiToW(StringUtils::TryBytesToStringA(value, valueBytes)), valueName ? StringUtils::AnsiToW(valueName) : L"");
}

void RegLog::LogRawWide(HKEY hKey, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCWSTR valueName)
{
	Log(hKey, type, statusCode, StringUtils::TryBytesToStringW(value, valueBytes), valueName ? valueName : L"");
}

void RegLog::LogWithSubpath(HKEY hKeySrc, HKEY hKeyNew, std::wstring subpath, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value, std::wstring valueName)
{
	if (statusCode == ERROR_SUCCESS && !subpath.empty())
	{
		/* Only save the new hKey if registry was opened successfully. Otherwise,
		   the registry wasn't actually opened/created, and so we can just log
		   the attempt without updating our hKey map. */

		keys.insert_or_assign(hKeyNew, GetQualifiedPath(hKeySrc, subpath));
		Log(hKeyNew, type, statusCode, value, valueName);
	}
	else
		LogWithSubpath(hKeySrc, subpath, type, statusCode, value, valueName);

}

void RegLog::LogWithSubpathRawAnsi(HKEY hKeySrc, HKEY hKeyNew, LPCSTR subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCSTR valueName)
{
	LogWithSubpath(hKeySrc, hKeyNew, StringUtils::TryAnsiToW(subpath), type, statusCode, StringUtils::AnsiToW(StringUtils::TryBytesToStringA(value, valueBytes)), valueName ? StringUtils::AnsiToW(valueName) : L"");
}

void RegLog::LogWithSubpathRawWide(HKEY hKeySrc, HKEY hKeyNew, LPCWSTR subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCWSTR valueName)
{
	LogWithSubpath(hKeySrc, hKeyNew, StringUtils::TryRawToW(subpath), type, statusCode, StringUtils::TryBytesToStringW(value, valueBytes), valueName ? valueName : L"");
}

void RegLog::LogWithSubpath(HKEY hKeySrc, std::wstring subpath, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value, std::wstring valueName)
{
	if (!subpath.empty())
		Log(GetQualifiedPath(hKeySrc, subpath), type, statusCode, value, valueName);
}

void RegLog::LogWithSubpathRawAnsi(HKEY hKey, LPCSTR subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCSTR valueName)
{
	LogWithSubpath(hKey, StringUtils::TryAnsiToW(subpath), type, statusCode, StringUtils::AnsiToW(StringUtils::TryBytesToStringA(value, valueBytes)), valueName ? StringUtils::AnsiToW(valueName) : L"");
}

void RegLog::LogWithSubpathRawWide(HKEY hKey, LPCWSTR subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value, DWORD valueBytes, LPCWSTR valueName)
{
	LogWithSubpath(hKey, StringUtils::TryRawToW(subpath), type, statusCode, StringUtils::TryBytesToStringW(value, valueBytes), valueName ? valueName : L"");
}