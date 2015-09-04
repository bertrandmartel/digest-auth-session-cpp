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
    ClientSocketHandler.cpp
    Client socket managing event handler

    @author Bertrand Martel
    @version 1.0
*/
#include "ClientSocketHandler.h"

#include "httpserverinter/IHttpClient.h"
#include "DigestManager.h"
#include "iostream"
#include "string.h"
#include "protocol/inter/http/IhttpFrame.h"
#include "sstream"

using namespace std;

ClientSocketHandler::ClientSocketHandler(DigestManager* digest_manager)
{
    this->digest_manager=digest_manager;

    unauthorized_page = QString("HTTP/1.1 401 Unauthorized\r\nContent-Type: text/html\r\nContent-Length: 297\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\r\n").toStdString()
            + "\"http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd\">\r\n"
            + "<HTML>\r\n"
            + "<HEAD>\r\n"
            + "<TITLE>Error</TITLE>\r\n"
            + " <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=ISO-8859-1\">\r\n"
            + "</HEAD>\r\n"
            + "<BODY><H1>401 Unauthorized.</H1></BODY>\r\n"
            + "</HTML>\r\n";

    internal_error_page = QString("HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 306\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\r\n").toStdString()
            + "\"http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd\">\r\n"
            + "<HTML>\r\n"
            + "<HEAD>\r\n"
            + "<TITLE>Error</TITLE>\r\n"
            + " <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=ISO-8859-1\">\r\n"
            + "</HEAD>\r\n"
            + "<BODY><H1>500 Internal Server Error.</H1></BODY>\r\n"
            + "</HTML>\r\n";

    login_success_page=QString("HTTP/1.1 200 Ok\r\nContent-Type: text/html\r\nContent-Length: 14\r\n\r\n<p>SUCCESS</p>").toStdString();
}

ClientSocketHandler::~ClientSocketHandler()
{
}

/**
 * called when an http response has been received from client
 *
 * @param client
 * 		client object
 * @param message
 * 		message delivered
 */
void ClientSocketHandler::onHttpResponseReceived(IHttpClient &client,Ihttpframe * frame,std::string peer_address){
}

/**
 * called when an http response has been received from client
 *
 * @param client
 * 		client object
 * @param message
 * 		message delivered
 */
void ClientSocketHandler::onHttpRequestReceived(IHttpClient &client,Ihttpframe * frame,std::string peer_address)
{
    cout << "Http request received for client " << peer_address << endl;

    if (strcmp(frame->getMethod().data(),"GET")==0 && strcmp(frame->getUri().data(),"/login")==0)
    {
        DigestInfo digest_info = digest_manager->process_digest(frame->getMethod(),frame->getUri(),frame->getHeaders());

        cout << "Receive digest response with status code : " << digest_info.get_status_code() << endl;

        std::string content = "";
        stringstream contentLength;

        if (digest_info.get_status_code()==401){

            content = unauthorized_page;
            contentLength << content.size();

            string response = QString("HTTP/1.1 401 Unauthorized\r\n").toStdString() +
                    "Content-Type: text/html\r\nContent-Length: " + contentLength.str() + "\r\n";

            if (digest_info.get_headers()->size()>0){

                for (std::map<std::string, std::string>::iterator iter=digest_info.get_headers()->begin(); iter!=digest_info.get_headers()->end(); ++iter){
                    response+=iter->first+": " +iter->second + "\r\n";
                }
            }

            response+=QString("\r\n").toStdString() + content;

            cout << "Sending response with status code 401 Unauthorized" << endl;

            client.sendHttpMessage(response);
        }
        else if (digest_info.get_status_code()==500){

            content = internal_error_page;
            contentLength << content.size();

            string response = QString("HTTP/1.1 500 Internal Server Error\r\n").toStdString() +
                    "Content-Type: text/html\r\nContent-Length: " + contentLength.str() + "\r\n";

            if (digest_info.get_headers()->size()>0){
                for (std::map<std::string,std::string>::iterator it=digest_info.get_headers()->begin(); it!=digest_info.get_headers()->end(); ++it)
                    response+=it->first+": " +it->second + "\r\n";
            }

            response+=QString("\r\n").toStdString() + content;

            cout << "Sending response with status code 500 Internal Server Error" << endl;

            client.sendHttpMessage(response);
        }
        else if (digest_info.get_status_code()==200){

            content = login_success_page;
            contentLength << content.size();

            string response = QString("HTTP/1.1 200 OK\r\n").toStdString() +
                    "Content-Type: text/html\r\nContent-Length: " + contentLength.str() + "\r\n";

            if (digest_info.get_headers()->size()>0){
                for (std::map<std::string,std::string>::iterator it=digest_info.get_headers()->begin(); it!=digest_info.get_headers()->end(); ++it)
                    response+=it->first+": " +it->second + "\r\n";
            }

            response+=QString("\r\n").toStdString() + content;

            cout << "Sending response with status code 200 OK" << endl;

            client.sendHttpMessage(response);
        }
    }
}
