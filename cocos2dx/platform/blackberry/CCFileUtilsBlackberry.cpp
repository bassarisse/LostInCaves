#include "CCFileUtilsBlackberry.h"
#include "platform/CCCommon.h"
#include "ccMacros.h"
#include "CCApplication.h"
#include "cocoa/CCString.h"
#include <unistd.h>

using namespace std;

NS_CC_BEGIN

FileUtils* FileUtils::sharedFileUtils()
{
    if (s_sharedFileUtils == NULL)
    {
        s_sharedFileUtils = new FileUtilsBlackberry();
        s_sharedFileUtils->init();
    }
    return s_sharedFileUtils;
}

FileUtilsBlackberry::FileUtilsBlackberry()
{}

bool FileUtilsBlackberry::init()
{
    _defaultResRootPath = "app/native/Resources/";
    return FileUtils::init();
}

string FileUtilsBlackberry::getWritablePath()
{
    // Let's write it in the current working directory's data folder
    char cwd[FILENAME_MAX] = {0};

    getcwd(cwd, FILENAME_MAX - 1);
    cwd[FILENAME_MAX-1] = '\0';

    std::string path = cwd;
    path += "/data/";

    return path;
}

bool FileUtilsBlackberry::isAbsolutePath(const std::string& strPath)
{
    if (strPath[0] == '/' || strPath.find(_defaultResRootPath) == 0)
    {
        return true;
    }
    return false;
}

bool FileUtilsBlackberry::isFileExist(const std::string& strFilePath)
{
    if (0 == strFilePath.length())
    {
        return false;
    }

    std::string strPath = strFilePath;
    if (strPath[0] != '/')
    { // Not absolute path, add the default root path at the beginning.
        if (strPath.find(_defaultResRootPath) != 0)
        {// Didn't find "assets/" at the beginning of the path, adding it.
            strPath.insert(0, _defaultResRootPath);
        }
    }

    return access(strPath.c_str(), F_OK) != -1 ? true : false;
}

NS_CC_END
