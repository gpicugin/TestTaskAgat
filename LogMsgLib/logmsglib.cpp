#include "logmsglib.h"
#include "qdir.h"
#include "qregularexpression.h"
#include <fstream>
#include <QDateTime>
#include <QDir>
static unsigned int globalLimit = 0;
static std::filesystem::path globalPath;
static unsigned int globalCurrentIndex = 0;

const unsigned int& testLib::getGlobalLimit()
{
    return globalLimit;
}

std::filesystem::path testLib::getGlobalPath()
{
    return globalPath;
}

const unsigned int& testLib::getCurrentIndex()
{
    return globalCurrentIndex;
}

bool testLib::setDirectory(const std::string& path, unsigned int limit)
{
    globalLimit = limit;
    globalPath = path;

    bool result;

    QDir dir(path.c_str(), "*.txt", QDir::SortFlag::Name);
    if(!dir.exists())
        result = std::filesystem::create_directory(path);
    else
        result = true;
    if(dir.isEmpty())
    {
        globalCurrentIndex = 0;
    }
    else
    {
        QRegularExpression rx("\\d+\\.txt");
        QStringList namesOfFiles = dir.entryList();
        QVector<int> numerics;
        foreach(QString name, namesOfFiles)
        {
            if( rx.match(name).hasMatch())
            {
                int tmp = QString(name.cbegin(), name.size() - 4).toInt();
                numerics << tmp;
            }
        }
        std::sort(numerics.begin(), numerics.end());
        globalCurrentIndex = numerics.at(numerics.size() - 1);
    }
    return result;
}

void testLib::log(const std::string& text)
{
    QDate date = QDate::currentDate();
    int day, month, year;
    date.getDate(&year, &month, &day);
    std::string strDay(std::to_string(day)),
                strMonth(std::to_string(month)), strYear(std::to_string(year));

    if(month < 10)
    {
        strMonth.insert(0, "0");
    }
    if(day < 10)
    {
        strDay.insert(0, "0");
    }

    std::string dateText = strDay + "-" + strMonth + "-" + strYear + "|" + text;

    std::filesystem::path pathFile(globalPath);
    pathFile /= std::string(std::to_string(globalCurrentIndex) + ".txt");

    QFileInfo info;
    info.setFile(pathFile);

    std::fstream fs;

    static bool firstStrFlag;
    if (info.size() == 0)
    {
        firstStrFlag = true;
    }
    else
    {
        firstStrFlag = false;
    }
    unsigned int newSize = info.size()  + dateText.size() + ((firstStrFlag) ? 0 : 1);

    if(newSize <= globalLimit)      // если влазит, то пишем
    {
        fs.open(pathFile, std::ios::app);
        if(!firstStrFlag)
        {
            fs << "\n";
        }
        fs << dateText;
        firstStrFlag = false;
    }
    else                                                    // если не влазит, то в новый файл
    {
        globalCurrentIndex++;
        std::filesystem::path pathFile2(globalPath);
        pathFile2 /= std::string(std::to_string(globalCurrentIndex) + ".txt");
        fs.open(pathFile2, std::ios::app);
        fs << dateText;
    }
    fs.close();
}
