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
#ifndef HTTPFRAME_H
#define HTTPFRAME_H

#include "IhttpFrame.h"

class httpframe : public Ihttpframe
{
public:
    httpframe();

    ~httpframe();

    /**
     * @brief getUri
     *      retrieve HTTP uri
     * @return
     */
     std::string getUri();

    /**
     * @brief setUri
     *      set http uri value
     * @param uri
     */
     void setUri(std::string uri);

    /**
     * @brief getMethod
     *      retrieve HTTP method
     * @return
     */
     std::string getMethod();

    /**
     * @brief setMethod
     *      set http method
     * @param method
     */
     void setMethod(std::string method);

    /**
     * @brief getBodyLength
     *      retrieve length of body
     * @return
     */
     std::string getBody();

    /**
     * @brief setBody
     *      set http body value
     * @param body
     */
    void setBody(std::string body);

    /**
     * @brief getQueryString
     *      retrieve query string value
     * @return
     */
     std::string getQueryString();

    /**
     * @brief setQueryString
     *          set http query string value
     * @param querystring
     */
     void setQueryString(std::string querystring);

    /**
     * @brief getStatusCode
     *      retrieve status code
     * @return
     */
     int getStatusCode();

    /**
     * @brief setStatusCode
     *      set status code value
     * @param statusCode
     */
     void setStatusCode(int statusCode);

    /**
     * @brief getHeaders
     *      retrieve http headers
     * @return
     */
     std::map< std::string,std::string > getHeaders();

    /**
     * @brief httpConsumer::getProcessState
     *      retrieve true if all data has been processed in current buffer
     * @return
     *
     */
     bool isFinishedProcessing();

    /**
     * @brief setFinishedProcessing
     *      set processing frame state
     * @param processing
     */
     void setFinishedProcessing(bool processing);

     /**
      * @brief setHeaders
      *     set http headers
      * @param header
      */
     void setHeaders(std::map<std::string,std::string> header);

private :

    /**
     * @brief querystring
     *      HTTP querystring
     */
    std::string querystring;

    /**
     * @brief uri
     *      uri of HTTP frame
     */
    std::string uri;

    /**
     * @brief method
     *      method of HTTP frame
     */
    std::string method;

    /**
     * @brief body
     *      body of HTTP frame
     */
    std::string body;


    /**
     * @brief statusNum
     *      status code of HTTP frame
     */
    int  statusCode;


    /**
     * @brief headers
     *      HTTP headers
     */
    std::map<std::string,std::string> headers;

    /**
     * @brief processFrame
     *      indicate that HTTP frame has finished processing
     */
    bool finishedProcessing;
};

#endif // HTTPFRAME_H
