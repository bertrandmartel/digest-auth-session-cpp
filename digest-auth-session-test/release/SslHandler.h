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
    SslHandler.h
    Manage ssl file formatted to QT ssl format cert

    @author Bertrand Martel
    @version 1.0
*/
#ifndef SSLHANDLER_H
#define SSLHANDLER_H
#include "QSslCertificate"
#include "QList"
#include "QSslKey"

/**
 * @brief The SslHandler class
 *      format ssl file cert to QT format
 */
class SslHandler
{
public:

    /**
     * @brief retrieveveCertFromFile
     *      retrieve ssl certificate type file
     * @param filename
     *      certificate file name (should be in pem format)
     * @return
     *      QSslcertificate format
     */
    static QSslCertificate retrieveCertFromFile(char * filename);


    static QSslKey retrieveKeyCertFile(char * filename,char * certPass);


    /**
     * @brief retrieveveCaCertListFromFile
     *      retrieve list of certification authorityto be used in server
     * @param filename
     *      certificates file name (should be in pem format)
     * @return
     *      QList< QSslCertificate > format
     */
    static QList< QSslCertificate > retrieveveCaCertListFromFile(char * filename);
};

#endif // SSLHANDLER_H
