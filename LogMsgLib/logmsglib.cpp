#include "logmsglib.h"
#include <fstream>
#include <QDateTime>
static unsigned int global_limit = 0;
static std::string global_path = "";
static unsigned int global_current_index = 0;

const unsigned int& getGlobalLimit()
{
    return global_limit;
}

const std::string& getPath()
{
    return global_path;
}

const unsigned int& getCurrentIndex()
{
    return global_current_index;
}

bool setDirectory(const std::string& path, unsigned int limit)
{
    global_limit = limit;
    global_path = path;
    return std::filesystem::create_directory(path);
}

void log(const std::string& text)
{
    QDate date = QDate::currentDate();
    int day, month, year;
    date.getDate(&year, &month, &day);
    std::string dateText = std::to_string(day) + "-" +
                           std::to_string(month) + "-" + std::to_string(year) + "|" + text;

    std::string pathFile = global_path + "\\" + std::to_string(global_current_index) + ".txt";

    QFileInfo info;
    info.setFile(pathFile.c_str());

    std::fstream fs;
    static unsigned int counter = 0;
    if(info.size() + dateText.size() <= global_limit)      // если влазит, то пишем
    {
        fs.open(pathFile, std::ios::app);
        if(counter != 0)
        {
            fs << "\n";
        }
        fs << dateText;
        counter++;
    }
    else                                                    // если не влазит, то в новый файл
    {
        global_current_index++;
        counter = 0;
        std::string pathFile2 = global_path + "\\" + std::to_string(global_current_index) + ".txt";
        fs.open(pathFile2, std::ios::app);
        fs << dateText;
        counter++;
    }
    fs.close();
}
