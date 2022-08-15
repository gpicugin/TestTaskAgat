QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testlogmsg.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LogMsgLib/release/ -lLogMsgLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LogMsgLib/debug/ -lLogMsgLib
else:unix: LIBS += -L$$OUT_PWD/../LogMsgLib/ -lLogMsgLib

INCLUDEPATH += $$PWD/../LogMsgLib
DEPENDPATH += $$PWD/../LogMsgLib
