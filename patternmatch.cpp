#include "patternmatch.h"
#include <iostream>

namespace PatternMatch
{
	std::wstring FindGUID(std::wstring in)
	{
		std::wregex pattern(L"[0-9a-fA-f]{8}-[0-9a-fA-f]{4}-[0-9a-fA-f]{4}-[0-9a-fA-f]{4}-[0-9a-fA-f]{12}");

		auto begin = std::wsregex_iterator(in.begin(), in.end(), pattern);
		auto end = std::wsregex_iterator();

		if (begin != end)
			return begin->str();
		
		return L"";
	}
}