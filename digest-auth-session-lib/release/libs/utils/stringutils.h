/**
    stringutils.h
    Utility funcions for http parser

    @author Bertrand Martel
    @version 1.0
*/
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "stringutils.h"
#include "vector"
#include "string"

/**
 * @brief The stringutils class
 *      String  utility functions
 */
class stringutils
{
public:

    /**
     * @brief split
     *      split a string with a character delimiter
     * @param s
     *      string to split
     * @param delim
     *      character delimiter
     * @return
     *      vector of splitted string
     */
    static std::vector<std::string> split(const std::string &s, char delim);

    /**
     * @brief split
     *      split a string with a character delimiter
     * @param s
     *      string to split
     * @param delim
     *      character delimiter
     * @param elems
     *      the same as vector list returned
     * @return
     *      vector of splitted string
     */
    static std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);

    /**
     * @brief isNum
     *      check if char * is numeric data
     * @param s
     *      char * input
     * @return
     *      true if data is numeric
     */
    static bool isNum(char *s);

};

#endif // STRINGUTILS_H
