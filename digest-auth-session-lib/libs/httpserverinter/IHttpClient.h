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
    IWebsocketClient.h

    Define generic template for http client socket

    @author Bertrand Martel
    @version 1.0
*/
#ifndef IHTTPCLIENT_H
#define IHTTPCLIENT_H
#include "string"
#include "QByteArray"

class IHttpClient
{

public :
    /**
     * Send http message to client
     *
     * @param string
     * 		Message to be sent to client
     * @return
     *		0 if success -1 if error
     */
    virtual void sendHttpMessage(std::string message)= 0;

    virtual void writeStringToSocket(std::string message)=0;

    virtual void writeByteArrayToSocket(const QByteArray array)=0;

    virtual void flush()=0;

    virtual void writeCharToSocket(char * data,int size)=0;

};

#endif // IHTTPCLIENT_H
