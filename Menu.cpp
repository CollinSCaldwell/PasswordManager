#include "Encryption.h"
#include "FileReader.h"
#include "Menu.h"
#include "Inherited.h"

using namespace helper;

int startUp(){
    char enteredCharacter = ' ';
    
    while(enteredCharacter != '0' && enteredCharacter != '1'){
        displayMainMenu();
        std::cout << "Input :";
        std::cin >> enteredCharacter;
        std::cin.clear();
        std::cin.ignore(20, '\n');
        
    }
    system("CLS");

    if(enteredCharacter == '0')
        return 0;
    else
    {
        return 1;
    }

}


void displayMainMenu(){
    system("CLS");
    
    std::cout << "---Password Management System---" << std::endl;
    std::cout << " 0: Quit                        " << std::endl;
    std::cout << " 1: Log in                      " << std::endl;
    std::cout << "--------------------------------" << std::endl;
}


bool logInMenu(){
    std::cout << "Enter your password here: ";
    std::string returned;
    getline(std::cin, returned);
    
    if(comparePasswords(FileHashHex,returned)){
        return 1;
    }
    return 0;
}


void wrongPasswordEntered(){
    system("cls");
    std::cout << "You entered the wrong password, please try again." << std::endl;
    system("pause");
}




void displayMainLoggedInMenu(){
    system("CLS");
    int input = -1;

    while(input != 0){
        do{
            system("CLS"); 
            std::cout << "---Password Management System---" << std::endl;
            std::cout << " 0 : Log out" << std::endl;
            std::cout << " 1 : Add a site" << std::endl;
            std::cout << " 2 : Delete a site" << std::endl;
            std::cout << " 3 : See a site password" << std::endl;
            std::cout << " 4 : Reset the master password" << std::endl;
            std::cout << "--------------------------------" << std::endl;
            std::cout << "Input :";
            std::cin >> input;
            if(std::cin.fail()){
                std::cin.clear();
                std::cin.ignore(255, '\n');
                input = -1;
            }
        }while(input == -1 || input < 0 || input > 4);
        if(input == 0){
            writeToFile("Passwords.TXT");
            ListOfSitesPasswordEncrypted.clear();
            break;
        }
        if(input == 1)
            addSite();
        if(input == 2)
            deleteSite();
        if(input == 3)
            pickSite();
        if(input == 4)
            resetPassword();
        writeToFile("Passwords.TXT");
    }
}


void addSite(){

    system("CLS");
    listSites();
    
    std::cout << "Enter the name of the site you would like to add (0 to exit): ";

    std::string entry;
    std::cin.clear();
    std::cin.ignore(255, '\n');
    getline(std::cin, entry);

    if(entry == "0"){
        return;
    }

    SiteAndPassword SandP;
    SandP.SiteNameEncrypted = entry;
    SandP.PasswordEncrypted = createPassword();
    SandP.PasswordEncrypted = SymmetricalEncryption(FileHashHexPassword, SandP.PasswordEncrypted);

    ListOfSitesPasswordEncrypted.push_back(SandP);
    listPasswordForSite(ListOfSitesPasswordEncrypted.size());
}


void deleteSite(){
    
    int input = -1;
    while(input == -1){
        system("CLS");
        if(!listSites()){
            system("pause");
            return;
        }

        std::cout << "Enter the number of the site you want to be deleted (0 to exit, -1 to delete all): ";
        std::cin >> input;
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(255, '\n');
            input = -1;
        }
        if(input == 0){
            return;
        }
        if(input == -1){
            ListOfSitesPasswordEncrypted.clear();
            return;
        }
        
        if(input < 0 || input > ListOfSitesPasswordEncrypted.size())
            input = -1;
        
    }

    ListOfSitesPasswordEncrypted.erase(ListOfSitesPasswordEncrypted.begin()+input-1);
}


void pickSite(){
    int input = -1;

    while(input == -1){

        system("CLS");
        if(!listSites()){
            system("pause");
            return;
        }

        std::cout << "List the site number you want the password for (0 to exit): ";

        std::cin >> input;
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(255, '\n');
            input = -1;
        }

        if(input < 0 || input > ListOfSitesPasswordEncrypted.size())
            input = -1;
        if(input == 0){
            return;
        }
    } 

    listPasswordForSite(input);


}




bool listSites(){
    if(ListOfSitesPasswordEncrypted.size() != 0){
        std::cout << "All the sites stored : " << std::endl << std::endl;
        for(int i = 0; i < ListOfSitesPasswordEncrypted.size(); i++){
            std::cout << i+1 << ": " << ListOfSitesPasswordEncrypted[i].SiteNameEncrypted << std::endl << std::endl;
        }
    }else{
        std::cout << "There are currently no sites stored" << std::endl << std::endl;
        return 0;
    }
    return 1;
}


void listPasswordForSite(int input){
    system("CLS");
    std::cout << "The Password for " << ListOfSitesPasswordEncrypted[input - 1].SiteNameEncrypted << " is '" << SymmetricalDecryption(FileHashHexPassword, ListOfSitesPasswordEncrypted[input - 1].PasswordEncrypted) << "'" << std::endl;
    system("pause");
}




bool setUpPassword(){
    
    system("CLS");
    std::cout << "There is no account currently setup, enter a password to setup an account, or press q to exit." << std::endl;
    std::cout << "Password: ";


    
    std::string entry;
    std::cin.clear();
    getline(std::cin, entry);


    if(entry == "q" || entry == "Q"){
        return 0;
    }

    FileHashHex = Hash(charStringToBinary(entry));
    writeNewPassword("Passwords.TXT");
    return 1;

}



bool resetPassword(){
    
    system("CLS");
    std::string entry = "";
        
    std::cin.clear();
    getline(std::cin, entry);
    while(entry != "Y" && entry != "y"){
        std::cout << "Are you sure you want to reset your password? (Y for yes, N for no): ";
    
        
        std::cin.clear();
        getline(std::cin, entry);


        if(entry == "N" || entry == "n"){
            return 0;
        }
    }
    
    bool test = false;
    while(!test){
        if(entry == "Q" || entry == "q"){
            return 0;
        }

        std::cout << "Enter current password: ";
    
        std::cin.clear();
        getline(std::cin, entry);

        test = comparePasswords(FileHashHex, entry);
        if(test){
            continue;
        }
        std::cout << "Wrong password, enter Q to quit or try again.\n" << std::endl;
        
    }

    std::cout << std::endl << "Enter new password: ";
    
    std::cin.clear();
    getline(std::cin, entry);


    for(int i = 0; i < ListOfSitesPasswordEncrypted.size(); i++){
        ListOfSitesPasswordEncrypted[i].PasswordEncrypted = SymmetricalDecryption(FileHashHexPassword, ListOfSitesPasswordEncrypted[i].PasswordEncrypted);
    }



    FileHashHex = Hash(charStringToBinary(entry));
    FileHashHexPassword = Hash(FileHashHex + entry);
    


    for(int i = 0; i < ListOfSitesPasswordEncrypted.size(); i++){
        ListOfSitesPasswordEncrypted[i].PasswordEncrypted = SymmetricalEncryption(FileHashHexPassword, ListOfSitesPasswordEncrypted[i].PasswordEncrypted);
    }


    std::cout << std::endl << "Success!\n";
    system("pause");

    return 1;

}