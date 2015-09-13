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
    DigestInfo.h
    Output digest information to be sent to client

    @author Bertrand Martel
    @version 1.0
*/
#ifndef DIGESTINFO_H
#define DIGESTINFO_H

#include "map"
#include "string"

/**
 * @brief The DigestInfo class
 *      Digest information used to build http request/response
 */
class DigestInfo
{
public:

    /**
     * @brief DigestInfo
     * @param status_code
     *      http status code
     * @param headers
     *      http headers
     */
    DigestInfo(int status_code,std::map< std::string,std::string> headers);

    ~DigestInfo();

    /**
     * @brief get_status_code
     *      Retrieve status code to return
     * @return
     */
    int get_status_code();

    /**
     * @brief get_headers
     *      Retrieve headers to return
     * @return
     */
    std::map< std::string,std::string >* get_headers();

private:

    /**
     * @brief status_code
     *      http status code
     */
    int status_code;

    /**
     * @brief headers
     *      http headers
     */
    std::map< std::string,std::string > headers;
};

#endif // DIGESTINFO_H
