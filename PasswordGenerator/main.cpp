#include <iostream>
#include<regex>
#include<string>
#include<fstream>
#include <unordered_map>
#include<random>

//GLOBAL VARIABLES
int MIN_TOKEN_LEN = 6;
int PASSWORDS_GENERATED = 20;

/* The purpose of this program is to generate random passwords from a given input text file.
 * This text file can be any file that contains enough words to meet the minimum requirement (4).
 * It should produce (20) passwords as an output and the passwords should consist of:
 *      (4) unique tokens from input.txt
 *      The first letter of ever token should be capitalized
 *      all remaining letters in the token should be lowercase
 *      The password will always end with the number 1
 *
 * EXTRA CREDIT:
 * use argc and argv for input
 * strip trailing punctuations (ie ".,!?")
 * Example output:
 * Billy Liom Doorknob Shampoo1
 * She The Tiger Salesman1
 * Trigger Unicorn Monkey Garage1
 * .
 * .
 * .
 *
 * This program will be my first attempt at using the regex library in order to filter a text file
 *
 * STEPS:
 * 1. read the input file
 * 2. match acceptable tokens
 * 3. append said tokens by lowercasing all the letters and removing any punctuation
 *
 * NOTES:
 * Regex expression for finding any words with length 3 or more: [a-zA-Z]{3,}
 * */

std::string getInputText() { // returns a string that contains the contents of input.txt
    //using file stream, create an object and open it to input.txt
    std::fstream fs;
    fs.open("input.txt");
    //check to make sure it is open, and use an iterator to assign a string variable to the information from the input.txt, then close the file and return the string
    if (fs.is_open()) {
        std::string inputFile;
        inputFile.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
        fs.close();
        return inputFile;
    } else {
        //No file was detected, throw an error, close the file and return with a "0"
        std::cout << "ERROR: File unable to open in method getInputText()";
        fs.close();
        return "0";
    }
}

std::string
getRegex() { // This will give our regex statement that will change based on the minimum token length global variable
    return ("[a-zA-Z]{" + std::to_string(MIN_TOKEN_LEN) + ",20}");
}// Regex token used -> /[a-zA-Z]{3,20}/g

//This is the main processing function in our program, and will take an input and convert it to the required format for the passwords and generate 20 of them.
void generatePasswords(std::string inputText) {
    //Variables
    std::string password;
    std::unordered_map<int, std::string> matchMap; //Map to contain our matches
    std::regex regexp(getRegex());
    std::smatch match;
    //Find our matches using the regex_search function, and store them to an unordered map
    while (std::regex_search(inputText, match, regexp)) {
        int iterator;
        for (auto x: match) {
            matchMap.emplace(iterator, x); //This will fill an unordered map with unique matches
            iterator++;
        }
        //Necessary for ending the while loop. Why? I'm not quite sure, regex is weird
        inputText = match.suffix().str();

    };
    std::cout << "Generating Passwords..." << std::endl << "Number of Potential Tokens: " << matchMap.size()
              << std::endl << "-------Passwords-------" << std::endl;
    //RNG
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());
    std::uniform_int_distribution<int> uniDist(0, matchMap.size());
    //This generates a random int each time it's called
    //match now contains our tokens, now we must create our passwords
    //1. Get the first token by calling a random key from our matchMap
    //2. Enter a for loop for the other 3 tokens so there can be spaces between them
    //3. lowercase any uppercase letters
    //4. Uppercase any letter directly after a space
    //5. Uppercase the first letter and add a 1 to the end of the string
    //6. Output and repeat 1-6 until enough passwords have been generated
    for (int i = 0; i < PASSWORDS_GENERATED; i++) {
        //Generate an unfiltered, random 4 tokens
        password = matchMap[uniDist(rng)];
        for (int j = 0; j < 3; j++) {
            password = password + " ";
            password.append(matchMap[uniDist(rng)]);
        }
        //4 tokens have been added together, now let's lowercase it
        for (int i = 0; i < password.size(); i++) {
            if (password[i] >= 65 && password[i] <= 90) {
                password[i] += 32;
            }
        }
        for (int i = 0; i < password.size(); i++) {
            if (password[i] == ' ') {
                password[i + 1] -= 32;
            }
        }
        password[0] -= 32;
        password += '1';

        std::cout << password << std::endl;

    }
}

int main(int argc, char *argv[]) {
    generatePasswords(getInputText());
    return 0;
}