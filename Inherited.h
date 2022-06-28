#ifndef INHERITED
#define INHERITED
#include <iostream>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cmath>



namespace helper
{

    std::string hexStringToBinary(std::string input);
    std::string hexToBinary(char input);



    std::string binaryStringToHex(std::string input);
    char binaryToHex(int input);


    std::string charStringToBinary(std::string input);
    std::string charToBinary(char);


    std::string binaryStringToCharString(std::string input);
    char binaryToChar(std::string);




    std::string multiplyBinary(std::string input1, std::string input2);


    std::string addBinary(std::string input1, std::string input2);




    std::string shiftBits(std::string input, int shiftAmount);



    char customBase64(int decimal);
}


#endif