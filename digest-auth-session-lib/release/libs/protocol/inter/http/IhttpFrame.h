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
    IHttpFrame.h
    HTTP frame template
    Regroup all http metadata for one http frame

    @author Bertrand Martel
    @version 1.0
*/
#ifndef IHTTPFRAME_H
#define IHTTPFRAME_H

#include "string"
#include "map"

/**
 * @brief The IHttpFrame class
 *      Interface defining all http metadata decoded from http decoder
 *
 */
class Ihttpframe{

public:
    /**
     * @brief getUri
     *      retrieve HTTP uri
     * @return
     */
    virtual std::string getUri()= 0;

    /**
     * @brief setUri
     *      set http uri value
     * @param uri
     */
    virtual void setUri(std::string uri)= 0;

    /**
     * @brief getMethod
     *      retrieve HTTP method
     * @return
     */
    virtual std::string getMethod()= 0;

    /**
     * @brief setMethod
     *      set http method
     * @param method
     */
    virtual void setMethod(std::string method)= 0;

    /**
     * @brief getBodyLength
     *      retrieve length of body
     * @return
     */
    virtual std::string getBody()= 0;

    /**
     * @brief setBody
     *      set http body value
     * @param body
     */
    virtual void setBody(std::string body)= 0;

    /**
     * @brief getQueryString
     *      retrieve query string value
     * @return
     */
    virtual std::string getQueryString()= 0;

    /**
     * @brief setQueryString
     *          set http query string value
     * @param querystring
     */
    virtual void setQueryString(std::string querystring)= 0;

    /**
     * @brief getStatusCode
     *      retrieve status code
     * @return
     */
    virtual int getStatusCode()= 0;

    /**
     * @brief setStatusCode
     *      set status code value
     * @param statusCode
     */
    virtual void setStatusCode(int statusCode)= 0;

    /**
     * @brief getHeaders
     *      retrieve http headers
     * @return
     */
    virtual std::map< std::string,std::string > getHeaders()= 0;

    /**
     * @brief httpConsumer::getProcessState
     *      retrieve true if all data has been processed in current buffer
     * @return
     *
     */
    virtual bool isFinishedProcessing()= 0;

    /**
     * @brief setFinishedProcessing
     *      set processing frame state
     * @param processing
     */
    virtual void setFinishedProcessing(bool processing)= 0;

};

#endif // IHTTPFRAME_H
