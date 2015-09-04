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
    ClientSocketHandler.h
    Client socket managing event handler

    @author Bertrand Martel
    @version 1.0
*/
#ifndef CLIENTSOCKETHANDLER_H
#define CLIENTSOCKETHANDLER_H

#include "httpserverinter/IClientEventListener.h"
#include "DigestManager.h"

/**
 * @brief The ClientSocketHandler class
 *      Client socket managing event handler
 */
class ClientSocketHandler :  public IClientEventListener
{
    public:

         /**
         * @brief ClientSocketHandler
         *      build client socket handler
         */
        ClientSocketHandler(DigestManager * digest_manager);

        ~ClientSocketHandler();

        /**
         * called when an http request has been received from client
         *
         * @param client
         * 		client object
         * @param message
         * 		message delivered
         */
        void onHttpRequestReceived(IHttpClient &client,Ihttpframe* consumer,std::string peer_address);

        /**
         * called when an http response has been received from client
         *
         * @param client
         * 		client object
         * @param message
         * 		message delivered
         */
        void onHttpResponseReceived(IHttpClient &client,Ihttpframe* consumer,std::string peer_address);

private:
        DigestManager * digest_manager;

        std::string unauthorized_page;

        std::string internal_error_page;

        std::string login_success_page;
};

#endif // CLIENTSOCKETHANDLER_H
