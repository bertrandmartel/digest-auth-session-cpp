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
    httpconsumer.cpp
    HTTP Consumer client
    This object permits to store http life cycle into your own design and monitor your parsing in a blocking or non-blocking socket configuration

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HTTPCONSUMER_H
#define HTTPCONSUMER_H

#include "string"
#include "QByteArray"
#include "map"
#include "IhttpFrame.h"
#include "vector"
#include "httpframe.h"

/**
 * @brief httpConsumer::httpConsumer
 *      Consumer client of http decoder<br/>
 *      This object permits to store http life cycle into your own design and monitor your parsing in a blocking or non-blocking socket configuration
 */
class httpconsumer
{
    public:

        ~httpconsumer();

        /**
         * @brief httpConsumer::httpConsumer
         *      Consumer client of http decoder<br/>
         *      This object permits to store http life cycle into your own design and monitor your parsing in a blocking or non-blocking socket configuration
         */
        httpconsumer();

        /**
         * @brief httpConsumer::clearBuffer
         *      clear current buffer
         */
        void clearBuffer();

        /**
         * @brief httpConsumer::appendToBuffer
         *      append some data to buffer
         *@param data
         *      data to be appended
         * @return
         *      pointer to data
         */
        int appendToBuffer(QByteArray* data);

        /**
         * @brief httpConsumer::getBuffer
         *      retrieve current buffer
         *@return
         */
        QByteArray getBuffer();

        /**
         * @brief httpConsumer::getProcessState
         *      retrieve true if all data has been processed in current buffer
         * @return
         *
         */
        bool isFinishedProcessing();

        /**
         * @brief setFinishedProcessing
         *      set processing frame state
         * @param processing
         */
        void setFinishedProcessing(bool processing);

        /**
         * @brief setDebug
         *      set debug mode for consumer
         * @param debug
         */
        void setDebug(bool debug);

        /**
         * @brief bodyIndex
         *      index of body in buffer element
         */
        int getBodyIndex();

        /**
         * @brief setBodyIndex
         *      set body index
         * @param bodyIndexArg
         */
        void setBodyIndex(int bodyIndexArg);

        /**
         * @brief bodyProcess
         *      get to know if HTTP body is to be parsed or not
         */
        bool getBodyProcess();

        /**
         * @brief setBodyProcess
         *      set HTTP body parsing state
         * @param processArg
         */
        void setBodyProcess(bool processArg);

        /**
         * @brief bodyLength
         *      length of body
         */
        int  getBodyLength();

        /**
         * @brief setBodyLength
         *      set body length
         * @param length
         */
        void setBodyLength(int length);

        /**
         * @brief httpState
         *      http parser current state
         */
        int  getHttpState();

        /**
         * @brief setHttpState
         *      set http parser state
         * @param http_state
         */
        void setHttpState(int http_state);

        /**
         * @brief isDebug
         *      debug state
         * @return
         */
        bool isDebug();

        /**
         * @brief getHttpFrameList
         *      retrieve http frame object list
         * @return
         */
        std::vector<httpframe> getHttpFrameList();

        /**
         * @brief httpConsumer::addNewHttpFrame
         *      add a new http frame in the list
         * @param frame
         */
        void addNewHttpFrame(httpframe frame);

        /**
         * @brief getCurrentHttpFrame
         *      retrieve current http frame object
         * @return
         */
        httpframe *getCurrentHttpFrame();

        /**
         * @brief setHttpFrameList
         *      set http frame list object
         * @param list
         *      pointer to vector of IHttp frames
         */
        void setHttpFrameList(std::vector<httpframe> list);

    private:

        /**
         * @brief buffer
         *      buffer used to store current unfinished http stream data (unfinished => not terminating with carriage return)
         */
        QByteArray    buffer;

        /**
         * @brief bodyIndex
         *      index of body in buffer element
         */
        int bodyIndex;

        /**
         * @brief bodyProcess
         *      get to know if HTTP body is to be parsed or not
         */
        bool bodyProcess;

        /**
         * @brief bodyLength
         *      length of body
         */
        int  bodyLength;

        /**
         * @brief httpState
         *      http parser current state
         */
        int  httpState;

        /**
         * @brief processFrame
         *      indicate that HTTP frame has finished processing
         */
        bool finishedProcessing;

        /**
         * @brief debug
         *      debug activation
         */
        bool debug;

        /**
         * @brief httpFrameList
         *      list of http frames
         */
        std::vector<httpframe> httpFrameList;

};

#endif // HTTPCONSUMER_H
