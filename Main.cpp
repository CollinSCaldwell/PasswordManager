//Author: Collin Caldwell
//Program: Basic test program to explore encryption algorithms 
//I wanted to design my own hashing algorithm to get an idea of how they truly work and how difficult it is to reverse them
//I also wanted to design my own symmetric encryption algorithm but I really just used the most basic of block ciphers which shift rows and columns
//If I were to do this project again I would definitely re-do the way I represent binary numbers as strings werent even the easiest to use
//I may use bitsets but I have also heard vectors of chars work very well for binary representations
//Currently the slowest function is the hashing function and that is a result of the many multiplications that I am doing which themselves are extremely slow, again due to my representation of binary strings
//I would also like to add salting and peppering to the hash function as while I understand them, implementing them would be fun
//I would also like to explore setting file names to the hashes of passwords, meaning multiple users can exits, though I do not know what the security implementations are of this
//Finally I would possibly like to see about exploring a forgot password option as that is something I find very interesting that even services like last pass dont fully describe how they implement 






#include "Encryption.h"
#include "FileReader.h"
#include "Menu.h"

int main(){

    while(startUp()){
        ListOfSitesPasswordEncrypted.clear();

        
        if(!readFile("Passwords.TXT")){
            setUpPassword();
            continue;
        }

        
        if(logInMenu()){
            displayMainLoggedInMenu();
        }else{
            wrongPasswordEntered();
        }
    }

    return 0;
}