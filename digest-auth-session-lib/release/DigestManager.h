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
    DigestManager.h
    Client socket managing event handler

    @author Bertrand Martel
    @version 1.0
*/
#ifndef DIGESTMANAGER_H
#define DIGESTMANAGER_H

#include "Digestinfo.h"
#include "QList"
#include "digeststruct.h"
#include "IDigestListener.h"
#include "QDateTime"
#include "map"

/* nonce structure used to store stateful data about nonce creation / lifetime */
typedef struct {
    char * nonce; // nonce value
    unsigned long expiring_date; // expiring date in millisecond since 1970
    unsigned int nonce_count; // nonce server count
    bool stale; // true if former nonce was rejected due to expiration (NOT IMPLEMENTED FOR NOW)
    char * session_id; // session id (only for COOKIE session type => this must match an entry in session structure below for this type of session)
} nonce;

/* session structure used to store stateful data about session entry for COOKIE session type*/
typedef struct{
    QDateTime session_start_date; // session record date
    bool validity; // validity of session entry
} session;

/**
 * @brief The DigestManager class
 *      Client socket managing event handler
 */
class DigestManager{

    public:

         /**
         * @brief DigestManager
         *      build client socket handler
         */
        DigestManager();

        ~DigestManager();

        /**
         * @brief process_digest
         *          process digest request / response according to method / uri / headers given in parameters. <br/>
         *          Will return following http content body according digest processing :
         *          <ul>
         *              <li>unauthorized_page : when client has not been authorized yet / or token expired / or authorization issue happened</li>
         *              <li>internal_error_page : when processing error occured</li>
         *              <li>expected_page : when client is fully authorized and session token is valid </li>
         *          </ul>
         * @param method
         *      http method
         * @param uri
         *      http uri
         * @param headers
         *      http headers
         * @param realm
         *      chosen realm for given uri
         * @return
         *      DigestInfo
         */
        DigestInfo process_digest(std::string method,std::string uri,std::map<std::string,std::string> * headers,std::string realm);

        /**
         * @brief DigestManager::remove_expired_nonces
         *      Remove all nonces in map that expired
         */
        void remove_expired_nonces();

        /**
         * @brief DigestManager::remove_expired_sessions
         *      Remove all session in map that expired
         */
        void remove_expired_sessions();

        /**
         * @brief getAuthenticationCookieField
         *      Retrieve HSID value from cookie http header
         *
         * @param cookieStr
         *      cookie header value
         * @return
         */
        std::string getAuthenticationCookieField(std::string cookieStr);

        /**
         * @brief splitHeader
         * @param header
         * @param delim
         * @return
         */
        std::map<std::string,std::string> splitHeader(std::string header,char delim);

        /**
         * @brief remove_session_for_headers
         *      Remove a session (cookie or digest)
         * @param headers
         * @return
         */
        int remove_session_for_headers(std::map<std::string,std::string> *headers);

        /**
         * @brief DigestManager::generateHandshakeProcess
         *      build digest authentication request
         * @param host
         *      host address
         * @param path_realm
         *      realm used for authentication
         * @return
         *      DigestInfo
         */
        DigestInfo generateHandshakeProcess(std::string host,std::string realm);

        /**
         * @brief DigestManager::generateRandomNum
         *      Generate random alpha numeric string
         * @param num
         *      length of random string
         * @return
         */
        std::string generateRandomNum(unsigned int num);

        /**
         * @brief DigestManager::processDigestResponse
         *      Process digest response request to authenticate and validate session cookie
         *
         * @param authorizationHeader
         *      header Authorization
         * @param cookieHeader
         *      Cookie header
         * @param realm
         *      realm used for this authentication
         * @param http_method_requested
         *      http method requested by client
         * @param uri_requested
         *      uri requested by client
         * @return
         *      DigestInfo
         */
        DigestInfo processDigestResponse(std::string authorizationHeader,std::string cookieHeader,std::string realm,std::string http_method_requested,std::string uri_requested);

        /**
         * @brief DigestManager::set_show_algorithm
         *      algorithm used in digest authentication will be sent to client if set to true (default)
         * @param show_in_response
         */
        void set_show_algorithm(bool show_in_response);

        /**
         * @brief DigestManager::set_digest_algorithm
         *      Set digest algorithm used for processing hash
         * @param algorithm
         */
        void set_digest_algorithm(digest_algorithm algorithm);

        /**
         * @brief DigestManager::getAlgorithm
         *      Retrieve digest get_algorithm used for processing hash
         * @return
         */
        digest_algorithm get_algorithm();

        /**
         * @brief DigestManager::set_nonce_timeout_millis
         *      set nonce timeout in milliseconds
         *  @param nonce_timeout
         */
        void set_nonce_timeout_millis(long nonce_timeout);

        /**
         * @brief DigestManager::get_nonce_timeout_millis
         *      Retrieve nonce timeout in milliseconds
         * @return
         */
        long get_nonce_timeout_millis();

        /**
         * @brief DigestManager::set_digest_listener
         *      Set digest listener
         * @param digest_listener
         */
        void set_digest_listener(IDigestListener* digest_listener);

        /**
         * @brief DigestManager::set_session_type
         *      Set session type (default is DIGEST session)
         * @param session
         */
        void set_session_type(session_type session);

        /**
         * @brief DigestManager::get_session_type
         *      Retrieve session type
         * @return
         */
        session_type get_session_type();

        /**
         * @brief DigestManager::set_max_session_time
         *      Set maximum session time for user
         * @param session_time
         */
        void set_max_session_time(int session_time);

        /**
         * @brief DigestManager::get_max_session_time
         *      Retrieve maximum session time for user
         * @return
         */
        int get_max_session_time();

        /**
         * @brief show_algorithm
         *      if set to true will show algorithm used in response to client
         */
        bool show_algorithm;

        /**
         * @brief algorithm
         *      algorithm used to compute hashes
         */
        digest_algorithm algorithm;

        /**
         * @brief nonce_timeout_millis
         *      nonce timeout in milliseconds (it is the session duration when digest session is used)
         */
        long nonce_timeout_millis;

        /**
         * @brief digest_listener
         *      listener for retrievinf username / password hash from storage
         */
        IDigestListener * digest_listener;

        /**
         * @brief session_t
         *      type of session used (can be DIGEST or COOKIE)
         */
        session_type session_t;

        /**
         * @brief session_time_seconds
         *      session duration for COOKIE session type (DIGEST session duration is set by nonce timeout)
         */
        int session_time_seconds;

        /**
         * @brief nonce_map
         *      map of nonces
         */
        std::map< std::string, nonce> nonce_map;

        /**
         * @brief session_map
         *      map of sessions (used only for COOKIE session)
         */
        std::map< std::string, session> session_map;
};

#endif // DIGESTMANAGER_H
