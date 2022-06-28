
#ifndef ENCRYPTION
#define ENCRYPTION
#include "Inherited.h"


std::string createPassword();


bool comparePasswords(std::string firstPassword, std::string secondPassword);


std::string Hash(std::string binaryInput);


std::string XOR(std::string firstString, std::string secondString);



std::string SymmetricalEncryption(std::string charKey, std::string toEncrypt);

std::string encryptBlock(std::string binaryKey, std::string toEncryptBinary, int roundLoops = 1);



std::string SymmetricalDecryption(std::string charKey, std::string toDecrypt);

std::string decryptBlock(std::string binaryKey, std::string toEncryptBinary, int roundLoops = 1);


#endif