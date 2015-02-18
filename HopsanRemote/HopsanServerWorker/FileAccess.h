//$Id$

#ifndef FILEACCESS_H
#define FILEACCESS_H

#include <string>
#include <vector>

class FileAccess
{
public:
    FileAccess();
    bool enterDir(const std::string &rDir);
    std::string currentDir() const;
    std::vector<std::string> findFilesWithSuffix(std::string suffix);
private:
    std::string mCurrentDir;
};

#endif // FILEACCESS_H
