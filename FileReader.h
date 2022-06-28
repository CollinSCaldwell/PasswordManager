
#ifndef FILE_READER
#define FILE_READER

#include "Inherited.h"

struct SiteAndPassword{
    std::string SiteNameEncrypted;
    std::string PasswordEncrypted;
};
extern std::string FileHashHex;
extern std::string FileHashHexPassword;
extern std::vector<SiteAndPassword> ListOfSitesPasswordEncrypted;



bool readFile(std::string);

void writeToFile(std::string);

void writeNewPassword(std::string filePath);

#endif
