#pragma once
#include <string>

class CLI
{
public:
	struct Options
	{
		std::string process = "";
		std::string dllPath = "";
		bool help	 = false;
		bool verbose = false;
		bool guids   = false;
		bool setOnly = false;
	};

	static Options ReadOptions(int argc, char** argv);
};