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
    httpconstants.h
    HTTP constants file
    Regroup all constants used for HTTP parser

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HTTPCONSTANTS_H
#define HTTPCONSTANTS_H
#include "vector"
#include "string"

/**
 * @brief The httpconstants class
 *      store HTTP parser constants
 */
class httpconstants
{

public:

    #define HTTP_HEADERS_CONTENT_LENGTH "Content-Length"

    #define HTTP_METHOD_GET    "GET"

    #define HTTP_METHOD_POST   "POST"

    #define HTTP_METHOD_PUT    "PUT"

    #define HTTP_METHOD_DELETE "DELETE"

    // struct for status code / status code phrase
    typedef struct statusCodeStruct
    {
        int         code_value;
        std::string code_phrase;

        statusCodeStruct(int code_value,std::string code_phrase) :
                      code_value(code_value), code_phrase(code_phrase){}
    } statusCode;

    /**
     * @brief http_status_code_list
     *      list of status code / code phrase
     */
    static std::vector<statusCodeStruct> http_status_code_list;

    /**
     * @brief static_init
     *      list of status code is statically stored : this must be called only once to avoid doublons (TODO : check if there is another way)
     */
    static void static_init(){
        http_status_code_list.push_back(statusCodeStruct(100,  "Continue"                        ));
        http_status_code_list.push_back(statusCodeStruct(101,  "Switching Protocols"             ));
        http_status_code_list.push_back(statusCodeStruct(200,  "OK"                              ));
        http_status_code_list.push_back(statusCodeStruct(201,  "Created"                         ));
        http_status_code_list.push_back(statusCodeStruct(202,  "Accepted"                        ));
        http_status_code_list.push_back(statusCodeStruct(203,  "Non-Authoritative Information"   ));
        http_status_code_list.push_back(statusCodeStruct(204,  "No Content"                      ));
        http_status_code_list.push_back(statusCodeStruct(205,  "Reset Content"                   ));
        http_status_code_list.push_back(statusCodeStruct(206,  "Partial Content"                 ));
        http_status_code_list.push_back(statusCodeStruct(300,  "Multiple Choices"                ));
        http_status_code_list.push_back(statusCodeStruct(301,  "Moved Permanently"               ));
        http_status_code_list.push_back(statusCodeStruct(302,  "Found"                           ));
        http_status_code_list.push_back(statusCodeStruct(303,  "See Other"                       ));
        http_status_code_list.push_back(statusCodeStruct(304,  "Not Modified"                    ));
        http_status_code_list.push_back(statusCodeStruct(305,  "Use Proxy"                       ));
        http_status_code_list.push_back(statusCodeStruct(307,  "Temporary Redirect"              ));
        http_status_code_list.push_back(statusCodeStruct(400,  "Bad Request"                     ));
        http_status_code_list.push_back(statusCodeStruct(401,  "Unauthorized"                    ));
        http_status_code_list.push_back(statusCodeStruct(402,  "Payment Required"                ));
        http_status_code_list.push_back(statusCodeStruct(403,  "Forbidden"                       ));
        http_status_code_list.push_back(statusCodeStruct(404,  "Not Found"                       ));
        http_status_code_list.push_back(statusCodeStruct(405,  "Method Not Allowed"              ));
        http_status_code_list.push_back(statusCodeStruct(406,  "Not Acceptable"                  ));
        http_status_code_list.push_back(statusCodeStruct(407,  "Proxy Authentication Required"   ));
        http_status_code_list.push_back(statusCodeStruct(408,  "Request Time-out"                ));
        http_status_code_list.push_back(statusCodeStruct(409,  "Conflict"                        ));
        http_status_code_list.push_back(statusCodeStruct(410,  "Gone"                            ));
        http_status_code_list.push_back(statusCodeStruct(411,  "Length Required"                 ));
        http_status_code_list.push_back(statusCodeStruct(412,  "Precondition Failed"             ));
        http_status_code_list.push_back(statusCodeStruct(413,  "Request Entity Too Large"        ));
        http_status_code_list.push_back(statusCodeStruct(414,  "Request-URI Too Large"           ));
        http_status_code_list.push_back(statusCodeStruct(415,  "Unsupported Media Type"          ));
        http_status_code_list.push_back(statusCodeStruct(416,  "Requested range not satisfiable" ));
        http_status_code_list.push_back(statusCodeStruct(417,  "Expectation Failed"              ));
        http_status_code_list.push_back(statusCodeStruct(500,  "Internal Server Error"           ));
        http_status_code_list.push_back(statusCodeStruct(501,  "Not Implemented"                 ));
        http_status_code_list.push_back(statusCodeStruct(502,  "Bad Gateway"                     ));
        http_status_code_list.push_back(statusCodeStruct(503,  "Service Unavailable"             ));
        http_status_code_list.push_back(statusCodeStruct(504,  "Gateway Time-out"                ));
        http_status_code_list.push_back(statusCodeStruct(505,  "HTTP Version not supported"      ));
    }
};

#endif // HTTPCONSTANTS_H
