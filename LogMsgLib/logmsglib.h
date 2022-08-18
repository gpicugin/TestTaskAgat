#ifndef LOGMSGLIB_H
#define LOGMSGLIB_H

#include "LogMsgLib_global.h"
#include <string>
#include <windows.h>
#include <filesystem>
#include <time.h>
#include <QFileInfo>
const unsigned int& LOGMSGLIB_EXPORT getGlobalLimit();

std::filesystem::path LOGMSGLIB_EXPORT getGlobalPath();

const unsigned int& LOGMSGLIB_EXPORT getCurrentIndex();

bool LOGMSGLIB_EXPORT setDirectory(const std::string& path, unsigned int limit);

void LOGMSGLIB_EXPORT log(const std::string& text);

#endif // LOGMSGLIB_H
