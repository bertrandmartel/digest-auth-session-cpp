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
    HttpServer.h

    Http server main process class

    manage incoming connections
    manage socket encryption for SSL socket
    manage process of incoming data from client socket

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include "protocol/inter/http/httpconsumer.h"
#include "protocol/http/httpdecoder.h"
#include "clientSocket.h"
#include "QSslCertificate"
#include "QList"
#include "QSslKey"
#include "httpserverinter/IClientEventListener.h"
#include <QtCore/QQueue>
#include "protocol/inter/http/httpframe.h"
/**
 * @brief The HttpServer class
 *      Http server main process class
 *
 * <ul>
 *  <li>manage incoming connections</li>
 *  <li>manage socket encryption for SSL socket</li>
 *  <li>manage process of incoming data from client socket</li>
 * </ul>
 */
class HttpServer: public QTcpServer
{

    Q_OBJECT
public:

    /**
     * @brief HttpServer
     *      Http server build object
     * @param parent
     */
    HttpServer(QObject * parent = 0);

    /**
    * @brief socketClientList
    *        socket client list
    */
    static std::map<QSslSocket*,ClientSocket > socketClientList;

    ~HttpServer();

    /**
    * @brief HttpServer::addClientEventListener
    *      add a client event listener to list
    * @param clientListener
    *      client listener
    */
    void addClientEventListener(IClientEventListener * clientListener);

    /**
     * @brief HttpServer::setSSL
     *      set Http server to secured Http server
     * @param use_ssl
     */
    void setSSL(bool use_ssl);

    /**
     * @brief HttpServer::setPublicCert
     *      set public server cert
     * @param cert
     *      public certificate
     */
    void setPublicCert(QSslCertificate cert);

    /**
     * @brief HttpServer::setCaCert
     *      set certification authoritycert
     * @param cert
     *      certification authority cert
     */
    void setCaCert(QList< QSslCertificate > cert);

    /**
     * @brief HttpServer::setPrivateCert
     *      set private certificate
     * @param cert
     *      private certificate
     */
    void setPrivateCert(QSslKey key);


    /**
     * @brief HttpServer::containsHttpProcessedFrames
     *      check if http frame list buffer already contains finished http processed frame or no
     * @param frameList
     *      list of http frames
     * @return
     */
    bool containsHttpProcessedFrames(std::vector<httpframe> frameList);

    /**
     * @brief HttpServer::set_debug
     *      activate / desactivate debug
     * @param use_ssl
     */
    void set_debug(bool state);

private slots:

    /**
     * @brief incomingData
     *      slot called when data comes from client socket
     */
    void incomingData();

    /**
     * @brief handleNewConnection
     *      called when a new connection pop up
     */
    void handleNewConnection();

    /**
     * @brief slot_connected
     *      called when a socket connect successfully to server
     */
    void slot_connected ();

    /**
     * @brief slot_disconnected
     *      called when a socket disconnect from server
     */
    void slot_disconnected ();

    void ready();

    void slot_error (QAbstractSocket::SocketError err);
    void slot_hostFound ();
    void slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *);
    void slot_stateChanged (QAbstractSocket::SocketState state);

    //bunch of slots used to check different state of SSL encryption process through QTNetwork processing
    void slot_encrypted ();
    void slot_encryptedBytesWritten (qint64 written);
    void slot_modeChanged (QSslSocket::SslMode mode);
    void slot_peerVerifyError (const QSslError &);
    void slot_sslErrors (const QList<QSslError> &);

private:

    /**
     * @brief nextPendingConnection
     *      overidde of next pending connection for queuing socket in a list
     * @return
     */
    QSslSocket* nextPendingConnection();

    /**
     * @brief incomingConnection
     *      new connection has come
     * @param socketDescriptor
     */
    void incomingConnection(int socketDescriptor);

    /**
     * @brief HttpServer::startServerEncryption
     *      add respective certificates for SSL encryption
     *
     * @param clientSocket
     *      ssl client socket
     */
    void startServerEncryption(QSslSocket* clientSocket);

    /**
     * @brief HttpServer::connectSocketSignals
     *      connect signals to slots : we take the maximum of slots here to manage errors quickly
     * @param clientSocket
     *      client socket incoming
     */
    void connectSocketSignals(QSslSocket* clientSocket);

    /**
     * @brief HttpServer::closeClientSocket
     *      close client socket function
     * @param socket
     *      client socket
     */
    void closeClientSocket(QSslSocket* socket);

    //ssl certs used to encrypt ssl socket

    /**
     * @brief localCertificate
     *      public certificate
     */
    QSslCertificate localCertificate;

    /**
     * @brief caCertificate
     *      certification authority list of certificates
     */
    QList< QSslCertificate > caCertificate;

    /**
     * @brief keyCertificate
     *      private certificate
     */
    QSslKey keyCertificate;

    /**
     * @brief consumer
     *      http consumer used to retrieve chunk of http frames
     */
    httpconsumer * consumer;

    /**
     * @brief decoder
     *          http decoder used to decode http frames streaming
     */
    httpdecoder decoder;

    /**
     * @brief debug
     *      debug state for Http server
     */
    bool debug;

    /**
     * @brief ssl
     *      define if Http protocol must use encryption or not
     */
    bool ssl;

    /**
     * @brief clientEventListenerList
     *      list of client event listener
     */
    std::vector<IClientEventListener*> clientEventListenerList;

    /**
     * @brief queue
     *      used to store connection in a list
     */
    QQueue<QSslSocket*> queue;
};


#endif // HTTPSERVER_H
