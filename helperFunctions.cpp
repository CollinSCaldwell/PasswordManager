#include "Inherited.h"
#include "Encryption.h"
#include "FileReader.h"
#include "Menu.h"

 

namespace helper
{


    //Takes in a string of hex characters and converts them to binary
    std::string hexStringToBinary(std::string input){
        std::string hexConversion = "";
        for(int i = 0; i < input.length(); i++){
            hexConversion += hexToBinary(input[i]);
        }
        return hexConversion;
    }

    //Converts a single hex character to binary
    std::string hexToBinary(char input){
        switch(toupper(input)){
            case '0' : return "0000";
            case '1' : return "0001";
            case '2' : return "0010";
            case '3' : return "0011";
            case '4' : return "0100";
            case '5' : return "0101";
            case '6' : return "0110";
            case '7' : return "0111";
            case '8' : return "1000";
            case '9' : return "1001";
            case 'A' : return "1010";
            case 'B' : return "1011";
            case 'C' : return "1100";
            case 'D' : return "1101";
            case 'E' : return "1110";
            case 'F' : return "1111";
        }
        return "";
    }




    //Converts a string of binary to hex characters
    std::string binaryStringToHex(std::string input){
        std::string hexConversion = "";
        for(int i = 0; i < input.length(); i+= 4){
            int decimalValue = 0;
            if(input[i] == '1'){
                decimalValue += 8;
            }
            if(input[i+1] == '1'){
                decimalValue += 4;
            }
            if(input[i+2] == '1'){
                decimalValue += 2;
            }
            if(input[i+3] == '1'){
                decimalValue += 1;
            }

            hexConversion += binaryToHex(decimalValue);
        }
        return hexConversion;
    }

    //Converts a binary string to a single hex character
    char binaryToHex(int input){
        
        switch(input){
            case 0 : return '0';
            case 1 : return '1';
            case 2 : return '2';
            case 3 : return '3';
            case 4 : return '4';
            case 5 : return '5';
            case 6 : return '6';
            case 7 : return '7';
            case 8 : return '8';
            case 9 : return '9';
            case 10 : return 'A';
            case 11 : return 'B';
            case 12 : return 'C';
            case 13 : return 'D';
            case 14 : return 'E';
            case 15 : return 'F';
        }
        return ' ';
    }




    //Converts a character ASCII string to binary
    std::string charStringToBinary(std::string input){
        std::string toReturn = "";
        for(int i = 0; i < input.length(); i++){
            toReturn += charToBinary(input[i]);
        }
        return toReturn;
    }

    //Converts a single ASCII character to binary
    std::string charToBinary(char input){
        std::string bits = "";

        int intValue = (int)input;

        for(int i = 8;  i > 0; i--){
            if(intValue - pow(2, i - 1) >= 0){
                intValue = intValue - pow(2, i - 1);
                bits += "1";
            }else{
                bits += "0";
            }
        }
        return bits;
    }




    //Converts a binary string to ASCII characters
    std::string binaryStringToCharString(std::string input){
        std::string toReturn = "";
        for(int i = 0; i < input.length(); i += 8){
            toReturn += binaryToChar(input.substr(i, 8));
        }
        return toReturn;
    }

    //Converts a single set of binary digits to a single char
    char binaryToChar(std::string bits){
        int returnable = 0;

        for(int i = 0; i < bits.length(); i++){
            if(bits[i] == '1'){
                returnable += pow(2, 7-i);
            }
        }

        return (char)returnable;
    }





    //Takes two binary strings and multiplies them together, possibly the slowest way to do this
    std::string multiplyBinary(std::string input1, std::string input2){
        std::string toReturn = "";

        if(input2[input2.length() - 1] == '0'){
            for(int i = 0; i < input2.length(); i++){
                toReturn += '0';
            }
        }else{
            toReturn = input1;
        }



        for(int i = 1; i < input2.length(); i++){
            
            std::string temporary = "";
            if(input2[input2.length() - 1 - i] == '1'){
                
                temporary = input1;
                for(int j = 0; j < i; j++){
                    temporary += '0';
                }
                
                toReturn = addBinary(temporary, toReturn);
                
            }

        }
        
        return toReturn;
    }


    //Takes two binary strings and adds them together, possibly the slowest way to do this
    std::string addBinary(std::string input1, std::string input2){
        std::string toReturn = "";

        int remainder = 0;
        toReturn = input1.length() > input2.length() ? input1 : input2;
        int length = input1.length() > input2.length() ? input2.length() : input1.length();


    
        if(input1.length() != input2.length()){
            if(input1.length() > input2.length()){
                input2.insert(0, "0");
            }else{
                input1.insert(0, "0");
            }
            length++;
        }

        for(int i = 0; i < length; i ++){
            
            if(input2[input2.length() - 1 - i] == '1'){
                
                if(input1[input1.length() - 1 - i] == '1'){
                    if(remainder == 1){
                        toReturn[toReturn.length() - 1 - i] = '1';
                    }else{
                        
                        toReturn[toReturn.length() - 1 - i] = '0';
                    }
                    remainder = 1;
                }else{
                    if(remainder == 1){
                        toReturn[toReturn.length() - 1 - i] = '0';
                    }else{
                        toReturn[toReturn.length() - 1 - i] = '1';
                    }
                }
            }else{
                if(input1[input1.length() - 1 - i] == '1'){
                    if(remainder == 1){
                        toReturn[toReturn.length() - 1 - i] = '0';
                    }else{
                        toReturn[toReturn.length() - 1 - i] = '1';
                    }
                }else{
                    if(remainder == 1){
                        toReturn[toReturn.length() - 1 - i] = '1';
                    }else{
                        toReturn[toReturn.length() - 1 - i] = '0';
                    }
                    remainder = 0;
                }
            }

        }

    

        if(remainder == 1){
            toReturn.insert(0, "1");
        }

        if(toReturn[0] == '1'){
            toReturn.insert(0, "0");
        }
        
        return toReturn;
    }






    //Takes a string and rotates it the amount specified by the second argument
    std::string shiftBits(std::string input, int shiftAmount){
        std::string toReturn = input;

        if(shiftAmount > 0){
            shiftAmount = shiftAmount % input.length();
            toReturn = "";
            toReturn = input.substr(input.length() - shiftAmount, shiftAmount);
            toReturn += input.substr(0, input.length() - shiftAmount);
        }
        if(shiftAmount < 0){
            toReturn = "";
            shiftAmount = abs(shiftAmount);
            shiftAmount = shiftAmount % input.length();
            toReturn = input.substr(shiftAmount, input.length() - shiftAmount);
            toReturn += input.substr(0, shiftAmount);
        }
        return toReturn;
    }





    //Takes in a decimal value and returns a character based on that decimal value
    char customBase64(int decimal){
        std::string indexer = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvexyz1234567890-_=+/?.>,<;:'`~!@#$%^&*()";
        decimal %= indexer.length();

        return indexer[decimal];
    }




}