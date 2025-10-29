#include "cli.h"

CLI::Options CLI::ReadOptions(int argc, char** argv)
{
	CLI::Options options;
	
	if (argc == 0) 
		return options;

	options.process = argv[0];

	for (int i = 1; i < argc; i++) 
	{
			 if (!std::strcmp(argv[i], "/v"))     options.verbose = true;
		else if (!std::strcmp(argv[i], "/h"))	  options.help = true;
		else if (!std::strcmp(argv[i], "/help"))  options.help = true;
		else if (!std::strcmp(argv[i], "/?"))	  options.help = true;
		else if (!std::strcmp(argv[i], "/guids")) options.guids = true;
		else if (!std::strcmp(argv[i], "/set"))   options.setOnly = true;
		else								      options.dllPath = argv[i];
	}

	return options;
}