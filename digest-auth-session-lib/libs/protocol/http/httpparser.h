/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Bertrand Martel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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
    bool isMethod(std::string data);

    /**
     * @brief isStatusNum
     *     check if input data is a valid HTTP status code
     * @param data
     *      data to be tested
     * @return
     *      true if data is valid HTTP status code
     */
    bool isStatusNum(std::string data);

    /**
     * @brief debug
     *      debug activation
     */
    bool debug;
};

#endif // HTTPPARSER_H
