#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

namespace stackmachine {

   inline bool findAndExcludeSymbol(std::string &src, std::string &symbol)
   {
	size_t found = src.find_first_of(symbol);
	if (found != std::string::npos) {
	    src.erase(found, 1);

	    return true;
	}

	return false;
    }

   inline bool findAndExcludeSymbol(std::string &src, std::string &&symbol)
   {
	size_t found = src.find_first_of(symbol);
	if (found != std::string::npos) {
	    src.erase(found, 1);

	    return true;
	}

	return false;
    }

    static std::vector<std::string> split(const std::string &line, char delimiter, unsigned int step = 1)
    {
        std::vector<std::string> ret{};
        std::stringstream sstream{line};
        std::string item;
        while (std::getline(sstream, item, delimiter)) {
	    ret.emplace_back(item);
        }      

       return ret;
    }

    inline bool equalsTo(const std::string &src, const std::string &s)
    {
	if (src.length() > s.length()) return false;

	return std::equal(src.begin(), src.end(), s.begin());
    }

}

#endif
