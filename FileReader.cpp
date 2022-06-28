#include "Encryption.h"
#include "FileReader.h"
#include "Menu.h"


std::string FileHashHex;
std::string FileHashHexPassword;
std::vector<SiteAndPassword> ListOfSitesPasswordEncrypted;

//Function used for general file reading customized to the format of the password file
//First reads the hex of the password entered 
//Then reads the names of the websties followed by the encrypted passwords
//Stores the site and password pairs in a special struct which itself is stored in a vector
bool readFile(std::string filePath){
    
    std::ifstream MainFile;
    MainFile.open(filePath);


    getline(MainFile, FileHashHex, '\n');
    if(FileHashHex == ""){
        MainFile.close();
        return 0;
    }
    
    for(std::string fileText; getline(MainFile, fileText);){
        SiteAndPassword SandP;

        SandP.SiteNameEncrypted = fileText;
        std::getline(MainFile, SandP.PasswordEncrypted, '\n');
        
        ListOfSitesPasswordEncrypted.push_back(SandP);
    }


    MainFile.close();

    return 1;
}

//Function used for general file reading customized to the format of the password file
//First writes the hex of the password enetered
//Then writes the website password combos to the files subsequently in the vector
void writeToFile(std::string filePath){
    std::ofstream MainFile;
    MainFile.open(filePath);

    MainFile << FileHashHex << '\n';

    for(int i = 0; i < ListOfSitesPasswordEncrypted.size(); i++){
        MainFile << ListOfSitesPasswordEncrypted[i].SiteNameEncrypted << '\n';
        MainFile << ListOfSitesPasswordEncrypted[i].PasswordEncrypted << '\n';
    }
    MainFile.close();
}

//Writes a new password hex to the file
//Clears the file before writing
//If the user is resetting their password, the old file information will be useless
//If there are passwords stored, they will already be within the SitePassword pair vector
void writeNewPassword(std::string filePath){
    std::ofstream MainFile;
    MainFile.open(filePath);

    MainFile << FileHashHex << '\n';

    MainFile.close();
    
}