#include "detours.h"

#include <windows.h>
#include <iostream>
#include <detours.h>
#include <map>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <psapi.h>

static RegLog* regLog;

#pragma region Hooks
#pragma region Create Hooks
static LSTATUS(WINAPI* Real_RegCreateKeyA)(
    HKEY, LPCSTR, PHKEY) = RegCreateKeyA;

static LSTATUS(WINAPI* Real_RegCreateKeyW)(
    HKEY, LPCWSTR, PHKEY) = RegCreateKeyW;

static LSTATUS(WINAPI* Real_RegCreateKeyExA)(
    HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM,
    const LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD) = RegCreateKeyExA;

static LSTATUS(WINAPI* Real_RegCreateKeyExW)(
    HKEY, LPCWSTR, DWORD, LPWSTR, DWORD, REGSAM,
    const LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD) = RegCreateKeyExW;

static LSTATUS(WINAPI* Real_RegCreateKeyTransactedA)(
    HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM,
    const LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD, HANDLE, PVOID) = RegCreateKeyTransactedA;

static LSTATUS(WINAPI* Real_RegCreateKeyTransactedW)(
    HKEY, LPCWSTR, DWORD, LPWSTR, DWORD, REGSAM,
    const LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD, HANDLE, PVOID) = RegCreateKeyTransactedW;
#pragma endregion

#pragma region Open Hooks
static LSTATUS(WINAPI* Real_RegOpenKeyA)(
    HKEY, LPCSTR, PHKEY) = RegOpenKeyA;

static LSTATUS(WINAPI* Real_RegOpenKeyW)(
    HKEY, LPCWSTR, PHKEY) = RegOpenKeyW;

static LSTATUS(WINAPI* Real_RegOpenKeyExA)(
    HKEY, LPCSTR, DWORD, REGSAM, PHKEY) = RegOpenKeyExA;

static LSTATUS(WINAPI* Real_RegOpenKeyExW)(
    HKEY, LPCWSTR, DWORD, REGSAM, PHKEY) = RegOpenKeyExW;

static LSTATUS(WINAPI* Real_RegOpenKeyTransactedA)(
    HKEY, LPCSTR, DWORD, REGSAM, PHKEY, HANDLE, PVOID) = RegOpenKeyTransactedA;

static LSTATUS(WINAPI* Real_RegOpenKeyTransactedW)(
    HKEY, LPCWSTR, DWORD, REGSAM, PHKEY, HANDLE, PVOID) = RegOpenKeyTransactedW;
#pragma endregion

#pragma region Set Hooks
static LSTATUS(WINAPI* Real_RegSetValueExA)(
    HKEY, LPCSTR, DWORD, DWORD, const BYTE*, DWORD) = RegSetValueExA;

static LSTATUS(WINAPI* Real_RegSetValueExW)(
    HKEY, LPCWSTR, DWORD, DWORD, const BYTE*, DWORD) = RegSetValueExW;

static LSTATUS(WINAPI* Real_RegSetKeyValueA)(
    HKEY, LPCSTR, LPCSTR, DWORD, LPCVOID, DWORD) = RegSetKeyValueA;

static LSTATUS(WINAPI* Real_RegSetKeyValueW)(
    HKEY, LPCWSTR, LPCWSTR, DWORD, LPCVOID, DWORD) = RegSetKeyValueW;
#pragma endregion
#pragma endregion

#pragma region Eat
#pragma region Eat Create
static LSTATUS WINAPI Eat_RegCreateKeyA(
    HKEY hKey,
    LPCSTR lpSubKey,
    PHKEY phkResult
) 
{
    LSTATUS status = Real_RegCreateKeyA(hKey, lpSubKey, phkResult);
    regLog->LogWithSubpathRawAnsi(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::CREATE, status);
    return status;
}

static LSTATUS WINAPI Eat_RegCreateKeyW(
    HKEY hKey,
    LPCWSTR lpSubKey,
    PHKEY phkResult
) 
{
    LSTATUS status = Real_RegCreateKeyW(hKey, lpSubKey, phkResult);
    regLog->LogWithSubpathRawWide(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::CREATE, status);
    return status;
}

static LSTATUS WINAPI Eat_RegCreateKeyExA(
    HKEY                        hKey,
    LPCSTR                      lpSubKey,
    DWORD                       Reserved,
    LPSTR                       lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition
) 
{
    LSTATUS status = Real_RegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
    regLog->LogWithSubpathRawAnsi(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::CREATE, status);
    return status;
}

static LSTATUS WINAPI Eat_RegCreateKeyExW(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition
) 
{
    LSTATUS status = Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
    regLog->LogWithSubpathRawWide(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::CREATE, status);
    return status;
}

static LSTATUS WINAPI Eat_RegCreateKeyTransactedA(
    HKEY                        hKey,
    LPCSTR                      lpSubKey,
    DWORD                       Reserved,
    LPSTR                       lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition,
    HANDLE                      hTransaction,
    PVOID                       pExtendedParemeter
) 
{
    LSTATUS status = Real_RegCreateKeyTransactedA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
    regLog->LogWithSubpathRawAnsi(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::CREATE, status);
    return status;
}

static LSTATUS WINAPI Eat_RegCreateKeyTransactedW(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition,
    HANDLE                      hTransaction,
    PVOID                       pExtendedParemeter
) 
{
    LSTATUS status = Real_RegCreateKeyTransactedW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
    regLog->LogWithSubpathRawWide(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::CREATE, status);
    return status;
}
#pragma endregion

#pragma region Eat Open
static LSTATUS WINAPI Eat_RegOpenKeyA(
    HKEY   hKey,
    LPCSTR lpSubKey,
    PHKEY  phkResult
) 
{
    LSTATUS status = Real_RegOpenKeyA(hKey, lpSubKey, phkResult);
    regLog->LogWithSubpathRawAnsi(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::OPEN, status);
    return status;
}

static LSTATUS WINAPI Eat_RegOpenKeyW(
    HKEY hKey,
    LPCWSTR lpSubKey,
    PHKEY phkResult
) 
{
    LSTATUS status = Real_RegOpenKeyW(hKey, lpSubKey, phkResult);
    regLog->LogWithSubpathRawWide(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::OPEN, status);
    return status;
}

static LSTATUS WINAPI Eat_RegOpenKeyExA(
    HKEY   hKey,
    LPCSTR lpSubKey,
    DWORD  ulOptions,
    REGSAM samDesired,
    PHKEY  phkResult
) 
{
    LSTATUS status = Real_RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    regLog->LogWithSubpathRawAnsi(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::OPEN, status);
    return status;

}

static LSTATUS WINAPI Eat_RegOpenKeyExW(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult
) 
{
    LSTATUS status = Real_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    regLog->LogWithSubpathRawWide(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::OPEN, status);
    return status;
}

static LSTATUS WINAPI Eat_RegOpenKeyTransactedA(
    HKEY   hKey,
    LPCSTR lpSubKey,
    DWORD  ulOptions,
    REGSAM samDesired,
    PHKEY  phkResult,
    HANDLE hTransaction,
    PVOID  pExtendedParemeter
) 
{
    LSTATUS status = Real_RegOpenKeyTransactedA(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
    regLog->LogWithSubpathRawAnsi(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::OPEN, status);
    return status;
}

static LSTATUS WINAPI Eat_RegOpenKeyTransactedW(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult,
    HANDLE  hTransaction,
    PVOID   pExtendedParemeter
) 
{
    LSTATUS status = Real_RegOpenKeyTransactedW(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
    regLog->LogWithSubpathRawWide(hKey, *phkResult, lpSubKey, RegEvent::RegEventType::OPEN, status);
    return status;
}
#pragma endregion

#pragma region Eat Set
static LSTATUS WINAPI Eat_RegSetValueExA(
    HKEY        hKey,
    LPCSTR      lpValueName,
    DWORD       Reserved,
    DWORD       dwType,
    const BYTE* lpData,
    DWORD       cbData) 
{
    LSTATUS status = Real_RegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    regLog->LogRawAnsi(hKey, RegEvent::RegEventType::SET, status, lpData, cbData, lpValueName);
    return status;
}

static LSTATUS WINAPI Eat_RegSetValueExW(
    HKEY        hKey,
    LPCWSTR     lpValueName,
    DWORD       Reserved,
    DWORD       dwType,
    const BYTE* lpData,
    DWORD       cbData) 
{
    LSTATUS status = Real_RegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    regLog->LogRawWide(hKey, RegEvent::RegEventType::SET, status, lpData, cbData, lpValueName);
    return status;
}

static LSTATUS WINAPI Eat_RegSetKeyValueA(
    HKEY    hKey,
    LPCSTR  lpSubKey,
    LPCSTR  lpValueName,
    DWORD   dwType,
    LPCVOID lpData,
    DWORD   cbData) 
{
    LSTATUS status = Real_RegSetKeyValueA(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
    regLog->LogWithSubpathRawAnsi(hKey, lpSubKey, RegEvent::RegEventType::SET, status, static_cast<const BYTE*>(lpData), cbData, lpValueName);
    return status;
}

static LSTATUS WINAPI Eat_RegSetKeyValueW(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    LPCWSTR lpValueName,
    DWORD   dwType,
    LPCVOID lpData,
    DWORD   cbData) 
{
    LSTATUS status = Real_RegSetKeyValueW(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
    regLog->LogWithSubpathRawWide(hKey, lpSubKey, RegEvent::RegEventType::SET, status, static_cast<const BYTE*>(lpData), cbData, lpValueName);    
    return status;
}
#pragma endregion
#pragma endregion

HRESULT PatchAllDetours(RegLog* rl)
{
	regLog = rl;
    
    if (DetourIsHelperProcess())
        return S_OK;

    DetourRestoreAfterWith();
    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());
    // CREATE
    DetourAttach(&(PVOID&)Real_RegCreateKeyA, Eat_RegCreateKeyA);
    DetourAttach(&(PVOID&)Real_RegCreateKeyW, Eat_RegCreateKeyW);
    DetourAttach(&(PVOID&)Real_RegCreateKeyExA, Eat_RegCreateKeyExA);
    DetourAttach(&(PVOID&)Real_RegCreateKeyExW, Eat_RegCreateKeyExW);
    DetourAttach(&(PVOID&)Real_RegCreateKeyTransactedA, Eat_RegCreateKeyTransactedA);
    DetourAttach(&(PVOID&)Real_RegCreateKeyTransactedW, Eat_RegCreateKeyTransactedW);
    // OPEN
    DetourAttach(&(PVOID&)Real_RegOpenKeyA, Eat_RegOpenKeyA);
    DetourAttach(&(PVOID&)Real_RegOpenKeyW, Eat_RegOpenKeyW);
    DetourAttach(&(PVOID&)Real_RegOpenKeyExA, Eat_RegOpenKeyExA);
    DetourAttach(&(PVOID&)Real_RegOpenKeyExW, Eat_RegOpenKeyExW);
    DetourAttach(&(PVOID&)Real_RegOpenKeyTransactedA, Eat_RegOpenKeyTransactedA);
    DetourAttach(&(PVOID&)Real_RegOpenKeyTransactedW, Eat_RegOpenKeyTransactedW);
    // SET
    DetourAttach(&(PVOID&)Real_RegSetValueExA, Eat_RegSetValueExA);
    DetourAttach(&(PVOID&)Real_RegSetValueExW, Eat_RegSetValueExW);
    DetourAttach(&(PVOID&)Real_RegSetKeyValueA, Eat_RegSetKeyValueA);
    DetourAttach(&(PVOID&)Real_RegSetKeyValueW, Eat_RegSetKeyValueW);

    DetourTransactionCommit();
    return S_OK;
}