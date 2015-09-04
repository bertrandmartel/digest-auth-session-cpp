/**
    httpparser.h
    HTTP Parser
    Will parse streaming data through parseHttp() method

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HTTPPARSER_H
#define HTTPPARSER_H
#include <QByteArray>
#include <map>
#include <string>
#include <vector>
#include "protocol/inter/http/httpconsumer.h"

/**
 * @brief The HttpParser class
 *      Main HTTP Parser<br/>
 *      Will parse streaming data
 */
class httpparser
{

public:
    httpparser();

    /**
     * @brief parseHttp
     *      parse streaming data
     *
     * @param data
     *      streaming data
     */
    void parseHttp(QByteArray* data,httpconsumer * consumer);

    /**
     * @brief setDebug
     *      set debug mode for http parser
     * @param debug
     */
    void setDebug(bool debug);

    /**
     * @brief getHeaders
     *      retrieve http headers
     * @return
     */
    std::map< std::string,std::string > *getHeaders();

private:

    /**
     * @brief isMethod
     *      check if input data is a valid HTTP rest method
     *
     * @param data
     *      data to be tested
     * @return
     *      true if data is valid REST HTTP method
     */
    bool isMethod(char * data);

    /**
     * @brief isStatusNum
     *     check if input data is a valid HTTP status code
     * @param data
     *      data to be tested
     * @return
     *      true if data is valid HTTP status code
     */
    bool isStatusNum(char * data);

    /**
     * @brief debug
     *      debug activation
     */
    bool debug;
};

#endif // HTTPPARSER_H
