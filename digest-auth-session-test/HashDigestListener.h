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
    HashDigestListener.h

    Listener for retrieving username / password hash from storage

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HASHDIGESTLISTENER_H
#define HASHDIGESTLISTENER_H

#include "string"
#include "IDigestListener.h"
#include "digeststruct.h"

class HashDigestListener : public IDigestListener {

    public:

    /**
     * @brief ClientSocketObj::ClientSocketObj
     *      Build one client
     */
    HashDigestListener();

    ~HashDigestListener();

    /**
     * @brief get_hash_for_username
     *      retrieve hash in database or in your configuration for specified username and realm
     * @return
     */
    std::string get_hash_for_username(std::string username,std::string realm,digest_algorithm algo);

};

#endif // HASHDIGESTLISTENER_H
