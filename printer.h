#pragma once
#include "cli.h"
#include "reglog.h"

class Printer
{
private:
	template<typename First, typename ... Strings>
	static int PrintErrorImpl(First err, const Strings& ... rest);
	static int PrintErrorImpl();

public:
	static void PrintTitle(CLI::Options options);
	static void PrintHelp(CLI::Options options);

	static void PrintRegLog(RegLog regLog, CLI::Options options);

	template<typename First, typename ... Strings>
	static int  PrintError(First err, const Strings& ... rest);
};

template<typename First, typename ... Strings>
int Printer::PrintError(First err, const Strings& ... rest)
{
	std::cout << "ERROR RegSvr32Debugger: ";
	return PrintErrorImpl(err, rest...);
}

template<typename First, typename ... Strings>
int Printer::PrintErrorImpl(First err, const Strings& ... rest)
{
	std::cout << err;
	return PrintErrorImpl(rest...);
}

inline int Printer::PrintErrorImpl()
{
	std::cout << std::endl;
	return -1;
}