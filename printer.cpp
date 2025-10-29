#include <iostream>
#include <set>
#include "printer.h"
#include "patternmatch.h"

void Printer::PrintTitle(CLI::Options options)
{
	std::cout << "/=============================================\\" << std::endl;
	std::cout << "========       RegSvr32Debugger       =========" << std::endl;
	std::cout << "\\==============================================/" << std::endl;
}

void Printer::PrintHelp(CLI::Options options)
{
	std::cout << "RegSvr32Debugger: Grab the CLSIDs and IIDs of COM Classes by hooking into DllRegisterServer." << std::endl;
	
	std::cout << std::endl;
	std::cout << std::endl;
	
	std::cout << "This compiled executable's name is regsvr32.exe. This is to trick COM Objects into thinking we're the real deal." << std::endl;
	std::cout << "           |" << std::endl;
	std::cout << "           |" << std::endl;
	std::cout << "           |" << std::endl;
	std::cout << "           |" << std::endl;
	std::cout << "           |" << std::endl;
	std::cout << "           V" << std::endl;
	std::cout << "Usage: regsvr32.exe <DLL or OCX> [/v, /guids, /set]" << "" << std::endl;

	std::cout << "     /?      " << "Display this message" << std::endl;
	std::cout << "     /v      " << "Verbose output" << std::endl;
	std::cout << "     /guids  " << "Only print the GUIDS (CLSID, IID, etc.) that were referenced in DllRegisterServer" << std::endl;
	std::cout << "     /set    " << "Don't log RegOpen* or RegCreate* operations" << std::endl;

	std::cout << std::endl;
}

void Printer::PrintRegLog(RegLog regLog, CLI::Options options)
{
	std::set<std::wstring> guids;

	for (RegEvent& event : regLog.Events())
	{
		if (options.guids)
		{
			std::wstring s = PatternMatch::FindGUID(event.path);
			if (!s.empty())
				guids.insert(s);
		}
		else
		{
			std::wstring name;

			switch (event.type)
			{
				case RegEvent::RegEventType::CREATE: name = L"CREATE "; break;
				case RegEvent::RegEventType::OPEN:   name = L"OPEN   ";	break;
				case RegEvent::RegEventType::SET:    name = L"SET    ";	break;
			}

			if (!options.setOnly || event.type == RegEvent::RegEventType::SET)
			{
				std::wcout << name << " " << event.path << std::endl;
				std::wcout << L"        " << event.valueName << ": " << event.value << std::endl;

				if (options.verbose)
					std::cout << "        Status Code " << event.statusCode << std::endl << std::endl;
			}
		}
	}

	for (auto& a : guids)
		std::wcout << "{" << a << "}" << std::endl;
}