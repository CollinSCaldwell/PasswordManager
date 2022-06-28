#include "Encryption.h"
#include "FileReader.h"
#include "Menu.h"
#include "Inherited.h"
#include <chrono>
#include <cstdlib>


using namespace helper;


//Create Password Function
//Returns a 13 character long password
//The first 3 characters will gauranteed be a combination of a number, a letter, and a symbol
//The next 10 characters will be entirely random between all the combinations of numbers, letters, and symbols
std::string createPassword(){
    std::string password = "";
    password += customBase64(rand() % 25 + 62);
    password += customBase64(rand() % 10 + 52);
    password += customBase64(rand() % 52); 
    for(int i = 0; i < 10; i++){
        password += customBase64(rand()); 
    }
    return password;
}






//Function meant to abstract the comparing of two strings meant to represent passwords
//This implementation specifically takes in a representation of a password hash and a password as its second argument, in char form 
//It then solves the FileHashHexPassword which is the key used to decrypt all the stored passwords
//It is insecure because it is stored in memory but if someone has access to memory then they can probably have a keylogger as well, rendering everything null
//It then compares the hash of the password, in binary, which returns as hex, to the already stored hash and returns the result 
bool comparePasswords(std::string firstPassword, std::string secondPassword){
    FileHashHexPassword = Hash(hexStringToBinary(firstPassword) + charStringToBinary(secondPassword));
    return Hash(charStringToBinary(secondPassword)).compare(firstPassword) == 0;
}





//Custom made hash function which does a fair job at obscuring data and making the process extremely lossy
//This is probably the slowest part of the cryptographic series of functions
//That is because of the binary multiplication that takes place as because I use a string representation of binary, arithmetic operations are custom made and slow
//Takes in any size string of binary characters and returns a 32 character hex hash
std::string Hash(std::string binaryInput){


    std::string hashed = "";

    if(binaryInput.length() < 128){
        while(binaryInput.length() < 128){
            binaryInput += binaryInput;
            
        }
        binaryInput = binaryInput.substr(0, 128);
        hashed = binaryInput;
        
        
    }else{
         
        
        while(binaryInput.length() > 256){
            
                hashed += addBinary(binaryInput.substr(0, 128), binaryInput.substr(128, binaryInput.length() > 256 ?  128 : binaryInput.length() - 128));
            
            if(hashed.length() > 128){
                hashed = XOR(hashed.substr(0, 128), hashed.substr(128, hashed.length() - 128));

            }
            binaryInput = binaryInput.substr(256, binaryInput.length() - 256);
            
        }


        if(binaryInput.length() > 128){
            hashed += addBinary(binaryInput.substr(0, 128), binaryInput.substr(128, binaryInput.length() - 128));
            if(hashed.length() > 128){
                hashed = XOR(hashed.substr(0, 128), hashed.substr(128, hashed.length() - 128));
            }
        }
        
        hashed = XOR(hashed, binaryInput);
        
    }

    std::string hashed_2 = hashed;

    for(int i = 0; i < 110; i += 3){
        int offset = i;
        
        offset += hashed[i] == '1' ? 8 : 0;
        offset += hashed[i+1] == '1' ? 4 : 0;
        offset += hashed[i+2] == '1' ? 2 : 0;
        offset += hashed[i+3] == '1' ? 1 : 0;
        offset = offset % 120;
        std::string XOROutput = XOR(hashed.substr(offset, 4), hashed.substr(i, 4));
        hashed_2[offset] = XOROutput[0];
        hashed_2[offset+1] = XOROutput[1];
        hashed_2[offset+2] = XOROutput[2];
        hashed_2[offset+3] = XOROutput[3];
        
        
        hashed = shiftBits(hashed, offset);
        hashed_2 = shiftBits(hashed_2, -offset);
        
        for(int j = 0; j < offset - i; j++){
            hashed_2 = addBinary(hashed_2, hashed.substr(0, 128));
            if(hashed_2.length() > 128){
                hashed_2 = XOR(hashed_2.substr(0, 128), hashed_2.substr(128, hashed_2.length() - 128));
            }
        }
        

    

    }

    hashed_2 = multiplyBinary(hashed_2.substr(0, 64), hashed_2.substr(64, 64));
    if(hashed_2.length() > 128){
        hashed_2 = hashed_2.substr(0, 128);
    }


    return binaryStringToHex(hashed_2);
}





//Cryptographic XOR function taking in two strings
//The first string is the base string to be XOR'd and the second one is used as a looping string
//If the size of the second string is under the first, it modolos against the length of the second string and continues until it traverses the length of the first string
//If the size of the second string is over, it goes until the length of the first string
std::string XOR(std::string firstString, std::string secondString){
    std::string toReturn = firstString;

    for(int i = 0; i < toReturn.length(); i++){
        if(toReturn[toReturn.length() - i] == '0' && secondString[secondString.length() - (i % secondString.length())] == '0'){
            toReturn[toReturn.length() - i] = '1';
            continue;
        }
        if(toReturn[toReturn.length() - i] == '0' && secondString[secondString.length() - (i % secondString.length())] == '1'){
            toReturn[toReturn.length() - i] = '0';
            continue;
        }
        if(toReturn[toReturn.length() - i] == '1' && secondString[secondString.length() - (i % secondString.length())] == '0'){
            toReturn[toReturn.length() - i] = '0';
            continue;
        }
        if(toReturn[toReturn.length() - i] == '1' && secondString[secondString.length() - (i % secondString.length())] == '1'){
            toReturn[toReturn.length() - i] = '1';
            continue;
        }
    }
    return toReturn;
}







//Initial function used to setup the encryption of an entire string of data
//The first argument is a character key which can be any length as it will be hashed
//The second argument is a character string which gets converted to binary and itself can be any length
//The function itself uses an encrypt block which is a way of abstracting the row and colum reorderings, which are bitwise not bytewise
//Most of the complexity of this function comes from finding out how much padding is needed for the last block, and creating the last block itself
//This block of course is encrypted as otherwise it would give away the length of the initially decrypted message
std::string SymmetricalEncryption(std::string charKey, std::string toEncrypt){


    toEncrypt = charStringToBinary(toEncrypt);
    int blocks = (toEncrypt.length() / 64) ;
    double extraTest = (toEncrypt.length() / 64.0) - blocks ;
    blocks ++;
    if(extraTest != 0){
        blocks += 1;
    }


    std::string stringBlocks[blocks];

    for(int i = 0; i < blocks - 2; i++){
        stringBlocks[i] = toEncrypt.substr(i * 64, 64);
    }

    stringBlocks[blocks - 2] = toEncrypt.substr(64 * (blocks - 2), toEncrypt.length() - (64 * (blocks - 2)));

    int fillerAmount = 64 - stringBlocks[blocks - 2].length();
    
    for(int i = 0; i < fillerAmount; i++){
        stringBlocks[blocks - 2] += "0";
    }

    std::string binaryAmount = "0";

    binaryAmount += fillerAmount - 64 >= 0 ? '1' : '0';    fillerAmount -= fillerAmount - 64 >= 0 ? 64 : 0;
    binaryAmount += fillerAmount - 32 >= 0 ? '1' : '0';    fillerAmount -= fillerAmount - 32 >= 0 ? 32 : 0;
    binaryAmount += fillerAmount - 16 >= 0 ? '1' : '0';    fillerAmount -= fillerAmount - 16 >= 0 ?  16 : 0;
    binaryAmount += fillerAmount - 8 >= 0 ? '1' : '0';     fillerAmount -= fillerAmount - 8 >= 0 ? 8 : 0;
    binaryAmount += fillerAmount - 4 >= 0 ? '1' : '0';     fillerAmount -= fillerAmount - 4 >= 0 ? 4 : 0;
    binaryAmount += fillerAmount - 2 >= 0 ? '1' : '0';     fillerAmount -= fillerAmount - 2 >= 0 ? 2 : 0;
    binaryAmount += fillerAmount - 1 >= 0 ? '1' : '0';     fillerAmount -= fillerAmount - 1 >= 0 ? 1 : 0;

    stringBlocks[blocks - 1] = binaryAmount + "00000000000000000000000000000000000000000000000000000000";

    int oneCounter = 0;
    for(int i = 0; i < 64; i++){
        oneCounter += stringBlocks[blocks -1 ][i] == '1' ? 1 : 0;
    }

    if(oneCounter % 2 == 1){
        for(int i = 0; i < 64; i++){
            stringBlocks[blocks- 1][i] = stringBlocks[blocks - 1][i] == '0' ? '1' : '0'; 
        }
    }
    
    


    std::string encrypted = "";
    std::string binaryKey = charStringToBinary(charKey);
    for(int j = 0; j < 2; j++){
        for(int i = 0; i < blocks; i++){
            stringBlocks[i] = encryptBlock(binaryKey, stringBlocks[i]);
        }
    }
    
    for(int i = 0; i < blocks; i++){
        encrypted += stringBlocks[i];
    }

    


    return binaryStringToHex(encrypted);
}




//Encrypt block function which takes the binary encryption key and the 64 bit block to encrypt as a parameter
//Using information within the block to encrypt, this function shifts the rows and columns a certain amount, doing two passes
std::string encryptBlock(std::string binaryKey, std::string toEncryptBinary, int roundLoops){

    if(roundLoops < 1)
        roundLoops = 1;
    std::string hashedKey = charStringToBinary(Hash(binaryKey));

    std::string shiftMatrices = hashedKey.substr(0, 64);
    std::string XORKey = hashedKey.substr(64, 64);


    int offsets[16];
    for(int i = 0; i < 16; i++){
        offsets[i] = 0;
        offsets[i] += shiftMatrices[(i*4)] == '1' ? 8 : 0;
        offsets[i] += shiftMatrices[(i*4)+1] == '1' ? 4 : 0;
        offsets[i] += shiftMatrices[(i*4)+2] == '1' ? 2 : 0;
        offsets[i] += shiftMatrices[(i*4)+3] == '1' ? 1 : 0;
    }

    std::string temporaryString = "";


    for(int rounds = 0; rounds < roundLoops + 1; rounds++){
        
        temporaryString = "";
        for(int i = 0; i < 4; i++){
            temporaryString = shiftBits(toEncryptBinary.substr(i * 16, 16), offsets[i + (rounds * 8)]);
            for(int j = 0; j < 16; j++){
                toEncryptBinary[j + (i * 16)] = temporaryString[j];
            }
            temporaryString = "";
        }

        toEncryptBinary = XOR(toEncryptBinary, XORKey);


        temporaryString = "";
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                temporaryString += toEncryptBinary.substr(((j) * 16) + i*4, 4);
            }

            temporaryString = shiftBits(temporaryString, offsets[i+(rounds * 8) + 4]);


            for(int j = 0; j < 4; j++){
                for(int k = 0; k < 4; k++){
                    toEncryptBinary[((j) * 16) + i*4 + k] = temporaryString[(j*4) + k];
                }
                
            }


            temporaryString = "";
        
        }

        toEncryptBinary = XOR(toEncryptBinary, XORKey);

    }





    return toEncryptBinary;
}







//Initial function used to setup the decryption of an entire string of data
//The first argument is a character key which can be any length as it will be hashed
//The second argument is a character string which gets converted to binary and itself has to be a length multiple of 64 when converted to binary
//The function itself uses a decrpyt block which is a way of abstracting the row and colum reorderings, which are bitwise not bytewise
//Most of the complexity of this function comes from finding out how much padding was used for the last block, and removing the padding itself
std::string SymmetricalDecryption(std::string charKey, std::string toDecrypt){
    toDecrypt = hexStringToBinary(toDecrypt);
    
    int blocks = toDecrypt.length() / 64;

    std::string stringBlocks[blocks];
    std::string binaryCharKey = charStringToBinary(charKey);
    for(int i = 0; i < blocks; i++){
        stringBlocks[i] = decryptBlock(binaryCharKey, toDecrypt.substr(i*64, 64));
    }
    for(int j = 0; j < 1; j++){
        for(int i = 0; i < blocks; i++){
            stringBlocks[i] = decryptBlock(binaryCharKey, stringBlocks[i]);
        }
    }


    int oneCounter = 0;
    for(int i = 0; i < 64; i++){
        oneCounter += stringBlocks[blocks - 1][i] == '1' ? 1 : 0;
    }
    if(oneCounter % 2 == 1){
        for(int i = 0; i < 64; i++){
            stringBlocks[blocks - 1][i] = stringBlocks[blocks - 1][i] == '0' ? '1' : '0';
        }
    }

    int binaryAmount = 0;
    binaryAmount += stringBlocks[blocks - 1][1] == '1' ? 64 : 0; 
    binaryAmount += stringBlocks[blocks - 1][2] == '1' ? 32 : 0;
    binaryAmount += stringBlocks[blocks - 1][3] == '1' ? 16 : 0;
    binaryAmount += stringBlocks[blocks - 1][4] == '1' ? 8 : 0;
    binaryAmount += stringBlocks[blocks - 1][5] == '1' ? 4 : 0;
    binaryAmount += stringBlocks[blocks - 1][6] == '1' ? 2 : 0;
    binaryAmount += stringBlocks[blocks - 1][7] == '1' ? 1 : 0;
    

    stringBlocks[blocks - 2] = stringBlocks[blocks - 2].substr(0, 64 - binaryAmount);

    toDecrypt = "";
    for(int i = 0; i < blocks - 1; i++){
        toDecrypt += stringBlocks[i];
    }




    return binaryStringToCharString(toDecrypt);   
}




//Decrypt block function which takes the binary encryption key and the 64 bit block to decrypt as a parameter
//Using information within the block to encrypt, this function shifts the rows and columns a certain amount, doing two passes
std::string decryptBlock(std::string binaryKey, std::string toEncryptBinary, int roundLoops){
    if(roundLoops < 1)  
        roundLoops = 1;
    
    std::string hashedKey = charStringToBinary(Hash(binaryKey));

    std::string shiftMatrices = hashedKey.substr(0, 64);
    std::string XORKey = hashedKey.substr(64, 64);



    int offsets[16];
    for(int i = 0; i < 16; i++){
        offsets[i] = 0;
        offsets[i] += shiftMatrices[(i*4)] == '1' ? 8 : 0;
        offsets[i] += shiftMatrices[(i*4)+1] == '1' ? 4 : 0;
        offsets[i] += shiftMatrices[(i*4)+2] == '1' ? 2 : 0;
        offsets[i] += shiftMatrices[(i*4)+3] == '1' ? 1 : 0;
    }





    std::string temporaryString = "";

    
    for(int rounds = roundLoops; rounds > -1; rounds--){
            
        temporaryString = "";

        toEncryptBinary = XOR(toEncryptBinary, XORKey);

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                temporaryString += toEncryptBinary.substr(((j) * 16) + i*4, 4);
            }

            temporaryString = shiftBits(temporaryString, -offsets[i+(rounds*8) + 4]);


            for(int j = 0; j < 4; j++){
                for(int k = 0; k < 4; k++){
                    toEncryptBinary[((j) * 16) + i*4 + k] = temporaryString[(j*4) + k];
                }
                
            }


            temporaryString = "";
        
        }






        toEncryptBinary = XOR(toEncryptBinary, XORKey);
        
        temporaryString = "";

        for(int i = 0; i < 4; i++){
            temporaryString = shiftBits(toEncryptBinary.substr(i * 16, 16), -offsets[i+(rounds*8)]);
            for(int j = 0; j < 16; j++){
                toEncryptBinary[j + (i * 16)] = temporaryString[j];
            }
            temporaryString = "";
        }
    }


    return toEncryptBinary;
}

