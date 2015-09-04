/**
    stringutil.cpp

    String functions

    @author Bertrand Martel
    @version 1.0
*/
#include "stringutil.h"
#include <sstream>
#include <iostream>

using namespace std;

std::string stringutil::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::vector<std::string> stringutil::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> stringutil::split(const std::string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
