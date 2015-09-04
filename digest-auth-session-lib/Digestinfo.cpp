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
    DigestInfo.cpp
    Output digest information to be sent to client

    @author Bertrand Martel
    @version 1.0
*/
#include "Digestinfo.h"

/**
 * @brief DigestInfo
 * @param status_code
 *      http status code
 * @param headers
 *      http headers
 */
DigestInfo::DigestInfo(int status_code,std::map< std::string,std::string> headers){
    this->status_code=status_code;
    this->headers=headers;
}

DigestInfo::~DigestInfo()
{
}

/**
 * @brief get_status_code
 *      Retrieve status code to return
 * @return
 */

int DigestInfo::get_status_code(){
    return status_code;
}


/**
 * @brief get_headers
 *      Retrieve headers to return
 * @return
 */

std::map< std::string,std::string > *DigestInfo::get_headers(){
    return &headers;
}

