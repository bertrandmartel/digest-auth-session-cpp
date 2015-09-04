/**
    fileutils.h
    Functions used to manage files

    @author Bertrand Martel
    @version 1.0
*/
#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "QByteArray"

class fileutils
{
public:

    /**
     * @brief fileutils::readFromFile
     *      read file data content and extract it into QByteArray
     * @param fileName
     *      file to extract
     * @return
     */
    static QByteArray readFromFile(char * fileName);


};

#endif
