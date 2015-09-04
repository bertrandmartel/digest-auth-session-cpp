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
