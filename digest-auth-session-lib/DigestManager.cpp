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
    DigestManager.cpp
    Client socket managing event handler

    @author Bertrand Martel
    @version 1.0
*/
#include "DigestManager.h"
#include "Digestinfo.h"

#include "httpserverinter/IHttpClient.h"
#include "iostream"
#include "sstream"
#include "string.h"
#include "protocol/inter/http/IhttpFrame.h"
#include "QString"
#include "utils/stringutil.h"
#include "QCryptographicHash"

using namespace std;

/**
 * @brief DigestManager::DigestManager
 *      Build digest manager
 */
DigestManager::DigestManager(){
    //seed for random generated string
    srand(time(0));

    //algorithm is sent to client by default (RFC2617 compliant)
    show_algorithm=true;

    //default algorithm is MD5
    algorithm=SHA1;

    //nonce timeout default is 10 minutes
    nonce_timeout_millis=10*60*1000;

    session_t=DIGEST;

    session_time_seconds=3600;
}

/**
 * @brief DigestManager::remove_expired_nonces
 *      Remove all nonces in map that expired
 */
void DigestManager::remove_expired_nonces(){
}

/**
 * @brief DigestManager::remove_expired_sessions
 *      Remove all session in map that expired
 */
void DigestManager::remove_expired_sessions(){
}


/**
 * @brief DigestManager::set_session_type
 *      Set session type (default is DIGEST session)
 * @param session
 */
void DigestManager::set_session_type(session_type session){
    this->session_t=session;
}

/**
 * @brief DigestManager::get_session_type
 *      Retrieve session type
 * @return
 */
session_type DigestManager::get_session_type(){
    return session_t;
}

/**
 * @brief DigestManager::set_nonce_timeout_millis
 *      set nonce timeout in milliseconds
 *  @param nonce_timeout
 */
void DigestManager::set_nonce_timeout_millis(long nonce_timeout){
    nonce_timeout_millis=nonce_timeout;
}

/**
 * @brief DigestManager::get_nonce_timeout_millis
 *      Retrieve nonce timeout in milliseconds
 * @return
 */
long DigestManager::get_nonce_timeout_millis(){
    return nonce_timeout_millis;
}

DigestManager::~DigestManager(){
}

/**
 * @brief DigestManager::set_digest_algorithm
 *      Set digest algorithm used for processing hash
 * @param algorithm
 */
void DigestManager::set_digest_algorithm(digest_algorithm algorithm) {
    this->algorithm=algorithm;
}

/**
 * @brief DigestManager::get_algorithm
 *      Retrieve digest algorithm used for processing hash
 * @return
 */
digest_algorithm DigestManager::get_algorithm(){
    return algorithm;
}

/**
 * @brief DigestManager::set_max_session_time
 *      Set maximum session time for user
 * @param session_time
 */
void DigestManager::set_max_session_time(int session_time){
    this->session_time_seconds=session_time;
}

/**
 * @brief DigestManager::get_max_session_time
 *      Retrieve maximum session time for user
 * @return
 */
int DigestManager::get_max_session_time(){
    return session_time_seconds;
}

/**
 * @brief remove_session_for_headers
 *      Remove a session (cookie or digest)
 * @param headers
 * @return
 */
int DigestManager::remove_session_for_headers(std::map<std::string,std::string> *headers){

    cout << "remove session id " << endl;

    std::map<std::string,std::string> map_val=*headers;

    if (session_t==COOKIE){
        std::string currentHsid="";

        if (map_val.find("Cookie")!=map_val.end()){
            currentHsid = getAuthenticationCookieField(headers->at("Cookie"));
        }

        if (strcmp(currentHsid.data(),"")!=0){

            if (session_map.find(currentHsid)!=session_map.end()) {
                session_map.erase(currentHsid);
                cout << "HSID successfully removed"<<endl;
                return 200;
            }
            else{
                cout << "HDSID header not registered" << endl;
            }
        }
        else{
            cout << "no HDSID header received" << endl;
        }
    } else {

        if (map_val.find("Authorization")!=map_val.end()){

            std::string authorizationHeader=headers->at("Authorization");

            map<string,string> subheaderList = splitHeader(authorizationHeader,',');

            if (subheaderList.find("opaque")==subheaderList.end()){
                cerr<< "No opaque subheader" << endl;
                return 500;
            }

            std::string opaque = subheaderList["opaque"];

            //check if peer address has an entry in map
            if (nonce_map.find(opaque)==nonce_map.end()){
                nonce_map.erase(opaque);
                cout << "Opaque successfully removed"<<endl;
                return 200;
            }else{
                cerr << "Opaque not found in list" << endl;
            }
        }else{
            cerr << "No authentication headers" << endl;
        }
    }

    return 500;
}

/**
 * called when an http response has been received from client
 *
 * @param client
 * 		client object
 * @param message
 * 		message delivered
 */
DigestInfo DigestManager::process_digest(std::string method,std::string uri,std::map<std::string,std::string> * headers,std::string realm){

    std::map<std::string,std::string> map_val=*headers;

    //############################################################################################

    bool authenticated = false;

    if (session_t==COOKIE){
        std::string currentHsid="";

        if (map_val.find("Cookie")!=map_val.end()){
            currentHsid = getAuthenticationCookieField(headers->at("Cookie"));
        }

        if (strcmp(currentHsid.data(),"")!=0){

            QDateTime now = QDateTime::currentDateTime();

            if (session_map.find(currentHsid)!=session_map.end() && session_map[currentHsid].validity) {
                if (((now.toMSecsSinceEpoch() - session_map[currentHsid].session_start_date.toMSecsSinceEpoch())<(session_time_seconds*1000))){
                     authenticated=true;
                 }
                else{
                    cout << "session id " << currentHsid << " expired" << endl;
                    session_map.erase(currentHsid);
                }
            }
        }
        else{
            cout << "no HDSID header received" << endl;
        }
    }

    //proces digest response if Authroization and Cookie header are present
    if (!authenticated && map_val.find("Authorization")!=map_val.end()){

        std::string authentication=headers->at("Authorization");
        std::string cookie="";

        if (map_val.find("Cookie")!=map_val.end()){
            cookie=headers->at("Cookie");
        }

        cout << "[DIGEST MANAGER] PROCESS RESPONSE " << endl;
        return processDigestResponse(authentication,cookie,realm,"GET",uri);
    }

    //process digest handshake if not authenticated
    if (!authenticated){

        if (headers->count("Host")>0){

            std::string host=headers->at("Host");

            cout << "[DIGEST MANAGER] PROCESS REQUEST" << endl;

            return generateHandshakeProcess(host,realm);
        }
        else{

            cout  << "ERROR : No host header found in request" << endl;

            map<string,string> headers_ret;
            DigestInfo digest_response(500,headers_ret);

            return digest_response;
        }
    }

    if (authenticated){

        map<string,string> headers_ret;
        DigestInfo digest_response(200,headers_ret);
        return digest_response;
    }


    map<string,string> headers_ret;
    DigestInfo digest_response(401,headers_ret);
    return digest_response;
}

/**
 * @brief DigestManager::getAuthenticationCookieField
 *  parse content of cookie header to retrieve value of HSID value :
 *
 *  for input : "HSID=AYQEVn….DKrdst; Domain=.foo.com; Path=/; Expires=Wed, 13 Jan 2021 22:23:01 GMT; HttpOnly"
 *  will return : "AYQEVn….DKrdst"
 *
 * @param cookieStr
 * @return
 */
std::string DigestManager::getAuthenticationCookieField(std::string cookieStr){

    if (strcmp("",cookieStr.data())!=0){

        map<string,string> cookiesList = splitHeader(cookieStr,';');
        map< string,string >::iterator it = cookiesList.find("HSID");

        if(it != cookiesList.end()){

            return it->second;
        }
    }
    return "";
}

/**
 * @brief DigestManager::splitHeader
 *      Split header delimited with "delim" character
 * @param header
 *      input http header
 * @param delim
 *      character delimiter
 * @return
 */
std::map<std::string,std::string> DigestManager::splitHeader(std::string header,char delim){

    map<string,string> result;

    vector<string> commaSeparated = stringutil::split(header,delim);

    for (unsigned int i = 0; i  < commaSeparated.size();i++){

        if (QString(commaSeparated.at(i).data()).indexOf("=")!=0){

            std::string key   = stringutil::replaceAll(QString(commaSeparated.at(i).data()).trimmed().toStdString().substr(0,QString(commaSeparated.at(i).data()).trimmed().indexOf("=")),"\"","");
            std::string value = stringutil::replaceAll(QString(commaSeparated.at(i).data()).trimmed().toStdString().substr(QString(commaSeparated.at(i).data()).trimmed().indexOf("=")+1),"\"","");
            result[key]=value;
        }
    }
    return result;
}

/**
 * @brief DigestManager::set_show_algorithm
 *      algorithm used in digest authentication will be sent to client if set to true (default)
 * @param show_in_response
 */
void DigestManager::set_show_algorithm(bool show_in_response){
    show_algorithm=show_in_response;
}

/**
 * @brief DigestManager::generateHandshakeProcess
 *      build digest authentication request
 * @param host
 *      host address
 * @param path_realm
 *      realm used for authentication
 * @return
 *      http response
 */
DigestInfo DigestManager::generateHandshakeProcess(std::string host,std::string path_realm){

    std::string serverIp ="";

    if (QString(host.data()).indexOf(":")!=-1){
        serverIp=QString(host.substr(0,QString(host.data()).indexOf(":")).data()).trimmed().toStdString();
    }
    else{
        serverIp=QString(host.data()).trimmed().toStdString();
    }

    std::string realm = QString(QString(path_realm.data()).toUtf8().toBase64()).toStdString();

    std::string randomStr = generateRandomNum(62);

    unsigned long nonce_timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

    //generate nonce (algorithm used for generating nonce is not part of RFC)
    std::string nonce_value = QString(QCryptographicHash::hash(randomStr.data(), QCryptographicHash::Sha1).toHex()).toStdString();

    char *nonce_cstr = new char[nonce_value.length() + 1];
    strcpy(nonce_cstr, nonce_value.c_str());

    std::string opaque = generateRandomNum(62);

    //to be sure opaque is unique
    while (nonce_map.find(opaque)!=nonce_map.end()){
        opaque = generateRandomNum(62);
    }

    QDateTime current_date = QDateTime::currentDateTime();

    nonce nonce_item = {
        nonce_cstr,
        nonce_timestamp+nonce_timeout_millis,
        1,
        false,
        0
    };

    //store nonce in map for keeping authentication information for this peer
    nonce_map[opaque]=nonce_item;

    std::string clientId = "";

    if (session_t==COOKIE){

        clientId = generateRandomNum(62);

        //to be sure clientId is unique
        while (session_map.find(clientId)!=session_map.end()){
            clientId = generateRandomNum(62);
        }

        session_map[clientId].session_start_date=current_date;
        session_map[clientId].validity=false;

        char *client_id_cstr = new char[clientId.length() + 1];
        strcpy(client_id_cstr, clientId.c_str());

        nonce_map[opaque].session_id=client_id_cstr;
    }

    //std::string content = unauthorized_page;

    //stringstream contentLength;
    //contentLength << content.size();

    bool stale = nonce_map[opaque].stale;
    const char * stale_txt = "false";
    if (stale)
        stale_txt="true";

    //build digest www-authorization header
    std::string digest_header   = QString("").toStdString()      + "Digest "
        + "realm=\""            + realm                          + "\","
        + "qop=\""              + "auth-int"                     + "\","
        + "nonce=\""            + nonce_map[opaque].nonce  + "\","
        + "opaque=\""           + opaque + "\","
        + "stale=\""            + stale_txt                      + "\"";

    //add algorithm if wanted (always set to true according rfc)
    if (show_algorithm) {

        std::string algo_str = "";

        if (algorithm==MD5)
            algo_str="MD5";
        else if (algorithm==SHA1)
            algo_str="SHA1";

        digest_header+= QString("").toStdString() + ",algorithm=\"" + algo_str  + "\"";
    }

    map<string,string> headers_ret;

    headers_ret["WWW-Authenticate"]=digest_header;

    //build set of headers for digest response
    //std::string digestHandshake = QString("HTTP/1.1 401 Unauthorized\r\n").toStdString()
    //    + "WWW-Authenticate: " + digest_header            + "\r\n"
    //    + "Content-Type: "     + "text/html"              + "\r\n";
    //"Content-Length: "   + contentLength.str()      + "\r\n";

    if (session_t==COOKIE){

        //headers_ret["Set-Cookie"]="authentication=running";

        std::string cookie=QString("").toStdString() + "HSID=" + clientId + "; Domain=" + serverIp + "; Path=/;";

        headers_ret["Set-Cookie"]=cookie;

        //digestHandshake+=QString("").toStdString()
        //+ "Set-Cookie: "       + "authentication=running" + "\r\n"
        //+ "Set-Cookie: "       + "HSID=" + clientId + "; Domain=" + serverIp + "; Path=/; HttpOnly\r\n" + "\r\n";
    }
    /*
    else{
        digestHandshake+=QString("\r\n").toStdString();
    }
    */

    DigestInfo response_digest(401,headers_ret);

    //std::string fullResponse = digestHandshake + content;

    return response_digest;
}

/**
 * @brief DigestManager::generateRandomNum
 *      Generate random alpha numeric string
 * @param num
 *      length of random string
 * @return
 */
std::string DigestManager::generateRandomNum(unsigned int num){

    std::string str;

    const char alphanum[] =
       "0123456789"
       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
       "abcdefghijklmnopqrstuvwxyz";

    int stringLength = sizeof(alphanum) - 1;

    for(unsigned int i = 0; i < num;i ++){

        str += alphanum[rand() % stringLength];
    }
    return str;
}

/**
 * @brief DigestManager::set_digest_listener
 *      Set digest listener
 * @param digest_listener
 */
void DigestManager::set_digest_listener(IDigestListener* digest_listener){
    this->digest_listener=digest_listener;
}

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
 *      status code
 */
DigestInfo DigestManager::processDigestResponse(std::string authorizationHeader,std::string cookieHeader,std::string realm,std::string http_method_requested,std::string uri_requested){

    map<string,string> headers_ret;

    map<string,string> subheaderList = splitHeader(authorizationHeader,',');

    if (subheaderList.find("opaque")==subheaderList.end()){
        cout << "opaque property was not found in Authorization header response" << endl;
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    std::string opaque = subheaderList["opaque"];

    //check if peer address has an entry in map
    if (nonce_map.find(opaque)==nonce_map.end()){
        cout << "nonce wasnt requested for this client" << endl;
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    unsigned long nonce_timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

    //check nonce life time
    if (nonce_map[opaque].expiring_date<=nonce_timestamp){
        cout << "nonce expired" << endl;
        nonce_map.erase(opaque);
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    if (digest_listener==0) {
        cout << "digest listener was not defined. Cant process digest response" << endl;
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    if (strcmp(authorizationHeader.data(),"")!=0) {
//        Exemple => Digest realm="Ym91eWd1ZXN0ZWxlY29tYmhvbWUyMDE0AA==",
//                   qop="auth-int",
//                   nonce="f5cf2080b1352ef1c2a1aaa05bca0429",
//                   opaque="60ea993b2f20e83335d8d85c1cf054dc",
//                   username="qsd",
//                   uri="/login",
//                   response="4998d82770b0729dddf403f7d3b477bb",//                   nc=00000001,
//                   cnonce="testCnonce"

        //check authorization header properties
        if (subheaderList.find("cnonce")==subheaderList.end()){
            cout << "cnonce property was not found in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("username")==subheaderList.end()){
            cout << "username property was not found in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("response")==subheaderList.end()){
            cout << "response property was not found in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("qop")==subheaderList.end()){
            cout << "qop property was not found in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("nc")==subheaderList.end()){
            cout << "nc property was not found in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("digest-uri")==subheaderList.end()){
            cout << "digest-uri property was not found in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        //check that qop value is correct
        if (strcmp(subheaderList["qop"].data(),"auth-int")!=0){
            cout << "qop value is incorrect in digest response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        stringstream nonce_count;
        nonce_count << nonce_map[opaque].nonce_count;
        std::string nonce_count_str = std::string(8 - nonce_count.str().length(), '0') + nonce_count.str();

        nonce_map[opaque].nonce_count++;

        //check that nonce count value is correct
        if (strcmp(subheaderList["nc"].data(),nonce_count_str.data())!=0){
            cout << "nc value is incorrect in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        //check that request uri is right
        if (strcmp(subheaderList["digest-uri"].data(),uri_requested.data())!=0){
            cout << "digest-uri value is incorrect in Authorization header response" << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        std::string cnonce   = subheaderList["cnonce"];
        std::string username = subheaderList["username"];

        std::string hash2Str = QString(http_method_requested.data()).toStdString()  + ":" + uri_requested;
        std::string hash1 = digest_listener->get_hash_for_username(username,realm,algorithm);

        //check authentication data is not empty
        if (strcmp(hash1.data(),"")==0){
            cout << "empty hash returned from digest listener for username : " << username.data()  << endl;
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        if (session_t==DIGEST){
            hash1+=QString("").toStdString() + ":" + nonce_map[opaque].nonce + ":" + cnonce;
        }

        std::string hash2="";
        if (algorithm==MD5)
            hash2 = QString(QCryptographicHash::hash(hash2Str.data(), QCryptographicHash::Md5).toHex()).toStdString();
        else if (algorithm==SHA1)
            hash2 = QString(QCryptographicHash::hash(hash2Str.data(), QCryptographicHash::Sha1).toHex()).toStdString();

        std::string hash3Str = QString(hash1.data()).toLower().toStdString() + ":" +
                nonce_map[opaque].nonce  + ":" + nonce_count_str + ":" + cnonce + ":" + "auth-int" + ":" + QString(hash2.data()).toLower().toStdString();

        std::string hash3 ="";
        if (algorithm==MD5)
            hash3 = QString(QCryptographicHash::hash(hash3Str.data(), QCryptographicHash::Md5).toHex()).toStdString();
        else if (algorithm==SHA1)
            hash3 = QString(QCryptographicHash::hash(hash3Str.data(), QCryptographicHash::Sha1).toHex()).toStdString();

        std::string clientResponse = subheaderList["response"];

        if (session_t==COOKIE) {

            if (strcmp(cookieHeader.data(),"")!=0) {

                map<string,string> cookiesList   = splitHeader(cookieHeader       ,';');

                //check cookie header properties
                if (session_t==COOKIE && cookiesList.find("HSID")==subheaderList.end()){
                    cout << "HSID property was not found in Cookie header response" << endl;
                    DigestInfo digest_response(401,headers_ret);
                    return digest_response;
                }

                std::string hsid = cookiesList["HSID"];

                if (session_map.find(hsid)!=session_map.end()){

                    if (strcmp(nonce_map[opaque].session_id,hsid.data())==0){

                        cout << "Digest client recognized. Checking for response..." << endl;

                        if (strcmp(QString(hash3.data()).toUpper().toLocal8Bit().data(),QString(clientResponse.data()).toUpper().toLocal8Bit().data())==0){

                            cout << "Digest client with HSID " << hsid << " has been connected successfully" << endl;
                            session_map[hsid].validity=true;
                            nonce_map.erase(opaque);
                            DigestInfo digest_response(200,headers_ret);
                            return digest_response;
                        }
                        else{
                            cout << "Digest client with HSID " << hsid << " was rejected :/" << endl;
                        }
                    }
                    else{
                        cout << "Digest response was received for unregistered digest client !" << endl;
                    }
                }
                else{
                    cout << "unknown session id " << endl;
                }
            }
            else{
                cout << "No cookie header found" << endl;
            }
        }
        else {

            cout << "[DIGEST SESSION] Digest client recognized. Checking for response..." << endl;

            if (strcmp(QString(hash3.data()).toUpper().toLocal8Bit().data(),QString(clientResponse.data()).toUpper().toLocal8Bit().data())==0){

                DigestInfo digest_response(200,headers_ret);
                return digest_response;
            }
            else{
                cout << "Digest session authentication failed hash is invalid" << endl;
            }
        }
    }
    else {
        cout<< "Error while reading digest response..." << endl;
    }

    DigestInfo digest_response(401,headers_ret);
    return digest_response;
}
