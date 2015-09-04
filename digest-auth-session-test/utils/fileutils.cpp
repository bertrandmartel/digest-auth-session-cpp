/**
    fileutils.cpp
    Functions used to manage files

    @author Bertrand Martel
    @version 1.0
*/
#include "fileutils.h"
#include "QFile"
#include "iostream"

using namespace std;

/**
 * @brief fileutils::readFromFile
 *      read file data content and extract it into QByteArray
 * @param fileName
 *      file to extract
 * @return
 */
QByteArray fileutils::readFromFile(char * fileName)
{
    QFile file(fileName);

    QByteArray byteArray;

    if(file.open(QIODevice::ReadOnly))
    {
        byteArray = file.readAll();
        file.close();
    }
    else
    {
        cout << file.errorString().toStdString().data() << endl;
    }

    return byteArray;
}
