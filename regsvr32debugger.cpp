#include <iostream>
#include <windows.h>
#include <detours.h>
#include <map>
#include <cstdlib>
#include <vector>
#include "utils.h"
#include "detours.h"
#include "cli.h"
#include "printer.h"

typedef HRESULT(STDAPICALLTYPE * DllRegisterServer_t)();

int main(int argc, char** argv)
{
    CLI::Options cli = CLI::ReadOptions(argc, argv);

    RegLog log;
    PatchAllDetours(&log);

    if (cli.help || cli.dllPath.empty())
        Printer::PrintHelp(cli);

    if (cli.dllPath.empty())
        return 0;

    HMODULE dllHandle = LoadLibraryA(cli.dllPath.c_str());

    if (!dllHandle) 
        return Printer::PrintError("Could not load library ", cli.dllPath);

    DllRegisterServer_t dllRegisterServer = (DllRegisterServer_t)GetProcAddress(dllHandle, "DllRegisterServer");

    if (!dllRegisterServer)
        return Printer::PrintError("DllRegisterServer entrypoint not found in file ", cli.dllPath);

    Printer::PrintTitle(cli);

    HRESULT result = dllRegisterServer();

    if (result == S_OK)
        Printer::PrintRegLog(log, cli);

    std::cout << "DLLRegisterServer exited with status code " << result << std::endl << std::endl;
}