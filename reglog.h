#include <iostream>
#include <windows.h>
#include <detours.h>
#include <map>
#include <cstdlib>
#include <vector>


struct RegEvent
{
public:
	enum struct RegEventType
	{
		CREATE, OPEN, SET
	};

	RegEventType type;
	HRESULT statusCode;
	std::wstring path;
	std::wstring value;
	std::wstring valueName;

	RegEvent(RegEventType type, HRESULT statusCode, std::wstring path, std::wstring value, std::wstring valueName) : type(type), statusCode(statusCode), path(path), value(value), valueName(valueName) {}
};

#pragma once
class RegLog
{
private:
	std::map<HKEY, std::wstring> keys;
	std::vector<RegEvent> eventLog;

	std::wstring GetQualifiedPath(HKEY hKey);
	std::wstring GetQualifiedPath(HKEY hKey, std::wstring subpath);

public:
	RegLog();
	inline std::vector<RegEvent> Events() { return eventLog; }

	/*
	* `value` and `valueName` arguments can be empty strings (or nullptr, in the case of LPCSTR/LPCWSTR)
	* `subpath` can be empty or nullptr as well, in which case we do nothing
	*/

	void Log(std::wstring path, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value = L"", std::wstring valueName = L"");
	void Log(HKEY hKey, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value = L"", std::wstring valueName = L"");

	void LogRawAnsi(LPCSTR  path, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCSTR  valueName = nullptr);
	void LogRawWide(LPCWSTR path, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCWSTR valueName = nullptr);

	void LogRawAnsi(HKEY hKey, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCSTR  valueName = nullptr);
	void LogRawWide(HKEY hKey, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCWSTR valueName = nullptr);


	void LogWithSubpath(HKEY hKeySrc, HKEY hKeyNew, std::wstring subpath, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value = L"", std::wstring valueName = L"");
	void LogWithSubpathRawAnsi(HKEY hKeySrc, HKEY hKeyNew, LPCSTR  subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCSTR  valueName = nullptr);
	void LogWithSubpathRawWide(HKEY hKeySrc, HKEY hKeyNew, LPCWSTR subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCWSTR valueName = nullptr);

	void LogWithSubpath(HKEY hKeySrc, std::wstring subpath, RegEvent::RegEventType type, HRESULT statusCode, std::wstring value = L"", std::wstring valueName = L"");
	void LogWithSubpathRawAnsi(HKEY hKey, LPCSTR  subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCSTR  valueName = nullptr);
	void LogWithSubpathRawWide(HKEY hKey, LPCWSTR subpath, RegEvent::RegEventType type, HRESULT statusCode, const BYTE* value = nullptr, DWORD valueBytes = 0, LPCWSTR valueName = nullptr);
};
