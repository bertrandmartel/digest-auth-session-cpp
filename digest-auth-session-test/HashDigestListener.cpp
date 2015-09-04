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
    HashDigestListener.cpp

    Listener for retrieving username / password hash from storage

    @author Bertrand Martel
    @version 1.0
*/
#include "HashDigestListener.h"
#include "iostream"
#include "QString"
#include "QCryptographicHash"

using namespace std;

/**
 * @brief HashDigestListener::HashDigestListener
 *      Build one client
 */
HashDigestListener::HashDigestListener(){

}

HashDigestListener::~HashDigestListener(){

}

/**
 * @brief get_hash_for_username
 *      retrieve hash in database or in your configuration for specified username and realm
 * @return
 */
std::string HashDigestListener::get_hash_for_username(std::string username,std::string realm,digest_algorithm algo){

    // here you look for username entry in your database and extract hash of "ALGO(username:realm:password)"
    // where ALGO can be MD5 or SHA1 as specified in setAlgorithm() method

    std::string realm_encoded = QString(QString(realm.data()).toUtf8().toBase64()).toStdString();
    std::string hash2Str = QString(username.data()).toStdString() + ":" + realm_encoded + ":" + "test";

    std::string hash2="";

    if (algo==MD5){
        hash2 = QString(QCryptographicHash::hash(hash2Str.data(), QCryptographicHash::Md5).toHex()).toStdString();
    }
    else if (algo==SHA1){
        hash2 = QString(QCryptographicHash::hash(hash2Str.data(), QCryptographicHash::Sha1).toHex()).toStdString();
    }

    return hash2;
}
