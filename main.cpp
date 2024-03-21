#include <iostream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <fstream>
#include <sstream>

//Funtion to have the User create their username
std::string getUserName() {
    std::string username;
    std::cout << "Please enter your username: ";
    std::getline(std::cin,username);
    return username;
}

//Funtion to make sure the password is valid
bool isValidPassword(const std::string& password){
    bool hasNumber = false;
    bool hasSymbol = false;
    bool hasUpper = false;

    for (char ch : password){
        //Checks the password for a digit
        if (std::isdigit(ch)) {
             hasNumber = true;
        }
        //Checks the password for a symbol
        else if (std::ispunct(ch)) {
             hasSymbol = true;
        }
        //Checks the password for an uppercase letter
        else if (std::isupper(ch)) {
            hasUpper = true;
        }

        if (hasNumber && hasSymbol && hasUpper){
            return true;
        }
    }

    return false;
}

//Funtion to have the user create a password
std::string createPassword(){
    std::string password;
    std::cout << "Create a password that contains at least one symbol, one number, and one uppercase letter: ";
    std::getline(std::cin, password);

//Checks to see if the password has all the requested items
    while(!isValidPassword(password)){
        std::cout << "This is an Invalid password. Please include at least one symbol, one number, and one uppercase letter: ";
        std::getline(std::cin, password);
    }

    std::cout << "Perfect your password is \"" << password << "\" is this correct? (Y/N) ";
    char userResponse;
    std::cin >> userResponse;
    
    //Confirms from the user if the password is what they intended
    if (std::toupper(userResponse) == 'Y') {
        return password;
    }
    else {
        std::cout <<"Let's try this one more time. \n";
        return createPassword();
    } 
}

//Creates a dictionary for the user names and passwords linked to them
std::unordered_map<std::string, std::string> createDictionary(){

    //creates a file that has the database of past usernames created
    std::ifstream csvFile("users.csv");
    std::string line, key, value;
    std::unordered_map<std::string, std::string> userDatabase;

    //Creates a dictionary from the file that is being read
        if (csvFile.is_open()) {
        while (std::getline(csvFile, line)) {
            std::stringstream lineStream(line);
            if (std::getline(lineStream, key, ',') && std::getline(lineStream, value)) {
                userDatabase[key] = value;
            }
        }
        csvFile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }

    return userDatabase;
}

//Makes sure the username that was inputed is not already in use
std::string validateUserName(){
    std::string inputUser;
    std::cout << "Please log in to confirm username and password saved.\n";

    std::cout << "Enter your username: ";
    std::getline(std::cin,inputUser);

    return inputUser;
}

int main()
{
    //Creates dictionary for user database
    std::unordered_map<std::string, std::string> userDatabase = createDictionary();

    std::string username = getUserName();
    while (userDatabase.find(username) != userDatabase.end()){
        std::cout << "Username already in use please choose a new username. \n";
        username = getUserName();
    }
    std::string password = createPassword();

    userDatabase[username] = password;

    std::cout << "Thank you, your new username is \"" << username << "\" and your password is \"" << password << "\".\n";
    //Takes the new username and password and adds it to the dictionary
    std::ofstream csvFile("users.csv", std::ios::app);
    //Writes the new username and password to the file to be saved for later
    if (csvFile.is_open()) {
        csvFile << username << "," << password << std::endl;
        csvFile.close();
    } else {
        std::cout << "Unable to open file";
    }

    //User login
    std::string inputPass;
    std::getline(std::cin,inputPass); //only in place because for some reason any input from the user is skipped here. Used to eliminate problems for getting username from user to log in
    std::string inputUser = validateUserName();

    //Makes sure the Username is in the database
    if (userDatabase.find(inputUser) != userDatabase.end()){
        std::cout << "Password: ";
        std::getline(std::cin,inputPass);
        //Checks to see if Username and Password match
        if (inputPass == userDatabase[inputUser]){
            std::cout << "Success. Logged in";
        }
        else {
            std::cout << "Password incorrect";
        }
    }
    else {
        std::cout << "UserName not found";
    }

 
    return 0;
}