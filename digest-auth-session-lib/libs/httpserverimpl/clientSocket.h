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
    clientsocket.h

    http client object featuring one physical http connection

    @author Bertrand Martel
    @version 1.0
*/
#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QByteArray>
#include <QTcpSocket>
#include <QList>
#include "httpserverinter/IHttpClient.h"

class ClientSocket : public IHttpClient
{

public:

    /**
     * @brief ClientSocket::ClientSocket
     *      Build one client
     */
    ClientSocket();


    ~ClientSocket();

    /**
     * @brief getPeerAddress
     *      Retrieve client ip address
     * @return
     */
    std::string getPeerAddress();

    /**
     *  close websoclet client object
     *
     * @return
     * 		0 if success -1 if error
     */
    int close();

    /**
     * Send a message to HTTP client
     *
     * @param string
     * 		Message to be sent to client
     * @return
     *		0 if success -1 if error
     */
    void sendHttpMessage(std::string message);

    /**
     * @brief setSocketClient
     *      Define client socket for this object
     * @param clientSocket
     *      client socket
     */
    void setSocketClient(QTcpSocket * clientSocket);

    void writeStringToSocket(std::string message);

    void writeByteArrayToSocket(const QByteArray array);

    void flush();

    void writeCharToSocket(char * data,int size);

private:

    /**
     * @brief clientSocket
     *      client socket
     */
    QTcpSocket* clientSocket;
};

#endif // CLIENTSOCKET_H
