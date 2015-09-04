/**
    HttpDecoder.h
    HTTP Decoder main class
    Will decode according to CRLF presence or add data to http consumer buffer

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HTTPDECODER_H
#define HTTPDECODER_H

#include "protocol/inter/http/httpconsumer.h"
#include "QByteArray"

/**
 * @brief The HttpDecoder class
 *
 *      Http decoder main input class
 */
class httpdecoder
{

public :

    httpdecoder();

    /**
     * @brief httpDecode
     *      http decoder
     *
     * @param consumer
     *      http client consumer featuring connection monitoring
     * @param data
     *      non-blocking or blocking data stream
     */
    void httpdecode(httpconsumer * consumer,QByteArray* data);

};

#endif // HTTPDECODER_H
