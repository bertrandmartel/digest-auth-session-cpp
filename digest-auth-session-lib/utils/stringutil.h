#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include "string"
#include <vector>

class stringutil
{
public:

    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);

    static std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);

    static std::vector<std::string> split(const std::string &s, char delim);
};

#endif // STRINGUTIL_H
