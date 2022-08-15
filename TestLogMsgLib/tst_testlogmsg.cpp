#include <QtTest>
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <fstream>
#include <random>
#include <stdio.h>
#include <../LogMsgLib/logmsglib.h>
// add necessary includes here

class TestLogMsg : public QObject
{
    Q_OBJECT

public:
    TestLogMsg();
    ~TestLogMsg();

private slots:
    void test_setDirectoryPath();
    void test_setDirectoryPath_data();
    void test_log();
    void test_log_data();
    void cleanupTestCase();
};

TestLogMsg::TestLogMsg()
{

}

TestLogMsg::~TestLogMsg()
{

}

void TestLogMsg::test_setDirectoryPath()
{
    QFETCH(std::string, path);
    QFETCH(unsigned int, limit);

    setDirectory(path, limit);
    QString qPath(path.c_str());
    QFileInfo info(qPath);

    QCOMPARE(info.exists() && info.isDir(), true);
    QCOMPARE(limit, getGlobalLimit());
}

void TestLogMsg::test_setDirectoryPath_data()
{
    QTest::addColumn<std::string>("path");
    QTest::addColumn<unsigned int>("limit");
    unsigned int limit = 1024;
    QTest::newRow("test1") << std::string("C:\\Repos\\TestTaskAgat\\Log0") << limit;
}

void TestLogMsg::test_log()
{
    QFETCH(std::string, text);
    unsigned int currInxex = getCurrentIndex();
    std::string currLogFilePath = getPath() + "\\" + std::to_string(currInxex) + ".txt";
    QString qCurrLogFilePath(currLogFilePath.c_str());
    QFileInfo info1(qCurrLogFilePath);
    unsigned int prevSize = info1.size();
    log(text);
    info1.refresh();

    QDateTime dateTime = QDateTime::currentDateTime();
    QDate date = dateTime.date();
    int day, month, year;
    date.getDate(&year, &month, &day);
    std::string dateText = std::to_string(day) + "-" +
                           std::to_string(month) + "-" + std::to_string(year) + "|" + text;

    QFileInfo info2(QString(std::string(getPath() +  "\\" + std::to_string(currInxex + 1) + ".txt").c_str()));
    bool result = ((prevSize < info1.size()) || info2.exists());

    QVERIFY(result);
    QFileInfo& info = info2.exists() ? info2 : info1;
    QVERIFY(info.size() <= getGlobalLimit());

    std::fstream fstream( info2.exists() ? info2.filePath().toStdString() : info1.filePath().toStdString());
    fstream.seekg(-1,std::ios_base::end);       // go to one spot before the EOF

    bool keepLooping = true;
    while(keepLooping)
    {
        char ch;
        fstream.get(ch);                        // Get current byte's data

        if((int)fstream.tellg() <= 1)
        {                                       // If the data was at or before the 0th byte
            fstream.seekg(0);                   // The first line is the last line
            keepLooping = false;                // So stop there
        }
        else if(ch == '\n') {                   // If the data was a newline
            keepLooping = false;                // Stop at the current position.
        }
        else {                                  // If the data was neither a newline nor at the 0 byte
            fstream.seekg(-2,std::ios_base::cur);        // Move to the front of that data, then to the front of the data before it
        }
    }

    std::string lastLine;
    getline(fstream,lastLine);

    QCOMPARE(lastLine, dateText);
}

void TestLogMsg::test_log_data()
{
    QTest::addColumn<std::string>("text");
    std::mt19937 rand(9);
    for(int i = 0; i < 100; i++)
    {
        unsigned int sizeOfRandStr = rand() % 100 + 1;
        unsigned int valueOfRandStr = rand() % 9 + 1;
        std::string testStr(sizeOfRandStr, *std::to_string(valueOfRandStr).c_str());
        std::string testNumeric = "test" + std::to_string(i);
        QTest::newRow(testNumeric.c_str()) << testStr;
    }
}

void TestLogMsg::cleanupTestCase()
{
//    std::string filePath;
//    for(unsigned int i = 0; i <= getCurrentIndex(); i++)
//    {
//        filePath = getPath() + "\\" + std::to_string(i) + ".txt";
//        remove(filePath.c_str());
//    }
}

QTEST_MAIN(TestLogMsg)

#include "tst_testlogmsg.moc"
