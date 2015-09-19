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
    main.cpp

    Main test programm

    @author Bertrand Martel
    @version 1.0
*/
#include <QCoreApplication>
#include <iostream>
#include <QStringList>
#include <QDebug>
#include "string"
#include <sys/param.h>
#include "SslHandler.h"
#include "httpserverimpl/httpserver.h"
#include "ClientSocketHandler.h"
#include "HashDigestListener.h"
#include "DigestManager.h"

using namespace std;

static int port = 4343;
static string ip="127.0.0.1";
static bool useSSL = false;

#define ROOT_FOLDER       "~"
#define PUBLIC_CERT       ROOT_FOLDER "/digest-auth-session-cpp/certs/server/server.crt"
#define PRIVATE_CERT      ROOT_FOLDER "/digest-auth-session-cpp/certs/server/server.key"
#define CA_CERTS          ROOT_FOLDER "/digest-auth-session-cpp/certs/ca.crt"
#define PRIVATE_CERT_PASS "12345"

int main(int argc, char *argv[]){

    QCoreApplication a(argc, argv);


    //ignore SIGPIPE signal (broken pipe issue)
    signal(SIGPIPE, SIG_IGN);

    QStringList args = a.arguments();

    string ip ="127.0.0.1";

    if (args.size() >2){

        ip=args[1].toStdString();
        bool ok = false;
        int dec = args[2].toInt(&ok, 10);

        if (ok)
            port = dec;
    }

    HashDigestListener digest_listener;
    DigestManager digest_manager;
    digest_manager.set_digest_algorithm(ALGO_SHA1);
    digest_manager.set_session_type(SESSION_DIGEST);

    digest_manager.set_digest_listener(&digest_listener);

    ClientSocketHandler clientHandler(&digest_manager);

    //instance of HTTP server
    HttpServer server;
    server.set_debug(false);

    if (useSSL){
        //set secured HTTP server
        server.setSSL(true);

        cout << "setting server certs ..." << endl;

        //set public / private and certification authority list into http server object
        server.setPublicCert(SslHandler::retrieveCertFromFile(PUBLIC_CERT));
        server.setPrivateCert(SslHandler::retrieveKeyCertFile(PRIVATE_CERT,PRIVATE_CERT_PASS));
        server.setCaCert(SslHandler::retrieveveCaCertListFromFile(CA_CERTS));
    }

    server.addClientEventListener(&clientHandler);

    if (!server.listen(QHostAddress(ip.data()),port)) {
        qDebug() << "An error occured while initializing hope proxy server... Maybe another instance is already running on "<< ip.data() << ":" << port << endl;
        return -1;
    }

    cout << "Starting HTTP server on "<< ip.data()  << port << endl;

    return a.exec();
}
