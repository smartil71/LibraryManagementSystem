
/*------------------------------------------------------------------------
Program 1: Library Management System
Course: CS-251, Fall 2023, UIC
System: Visual Studio 2019 on Windows 10
Author: Sean Martil, starter code provided by Professors Koehler, Ordentlich, and Dey
------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

// Function declarations
void addBook(vector<string> &bookLibrary, vector<string> &isbnLibrary, vector<int>& statusLibrary, const string title, const string isbn, const int status);
void exportlibrary(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const string filename);
int getBookIndex(vector<string>& bookLibrary, const string title);
int getISBNindex(vector<string>& isbnLibrary, const string isbn);
string getStatus(vector<int>& statusLibrary, const int index);
void loadFromFile(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const string filename);
void parseBook(const string bookisbn, string& currtitle, string& currisbn, int& currstatus);
void printcheckoutstatus(vector<int> statusLibrary);
void printMenu();
void printtitle(vector<string> &bookLibrary, vector<string> &isbnLibrary, vector<int> &statusLibrary, const int index);
void removebook(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index);
void searchbook(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index);
bool validentry(vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index);
bool validisbn(string isbn);
bool validstatus(vector<int> statusLibrary, const int index);
bool validtitle(const string title);

int main()
{
    string command;

    string currtitle;
    string currisbn;
    int currstatus = 0;
    vector<string> bookLibrary;
    vector<string> isbnLibrary;
    vector<int> statusLibrary;
    vector<int> invalidLibrary;//stores indices of invalid books

    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        // Output the menu and acquire a user selection
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";
        getline(cin, command);
        cout << endl;

        // add single item to library
        if (command[0] == 'a' || command[0] == 'A') {
            string temp = "";
            cout << "What is the book title? \n";
            getline(cin, command);
            if (validtitle(command)) {
                temp = command;
                cout << "What is the 13-digit ISBN (with hyphens)? \n";
                getline(cin, command);
                temp = temp + ", " + command;
                // assigns currtitle and currisbn
                parseBook(temp, currtitle, currisbn, currstatus);
                // adds book and isbn to library
                addBook(bookLibrary, isbnLibrary, statusLibrary, currtitle, currisbn, currstatus);
                cout << "The Following Entry Was Added\n-----------------------------\n";
                printtitle(bookLibrary, isbnLibrary, statusLibrary, bookLibrary.size() - 1);
                currtitle = "";
                currisbn = "";
                currstatus = 0;
            }
            else {
                cout << "The book title cannot contain commas.\n";
            }
        }
        // clear library of all entries
        else if (command[0] == 'c' || command[0] == 'C') {
            bookLibrary.clear();
            isbnLibrary.clear();
            statusLibrary.clear();
            cout << "Your library is now empty.\n";
        }
        // prints all library books, isbn, and status
        else if (command[0] == 'd' || command[0] == 'D') {
            cout << "Your Current Library\n--------------------\n";
            if (bookLibrary.size() == 0) {
                cout << "The library has no books.\n";
            }
            else {
                for (size_t i = 0; i < bookLibrary.size(); i++) {
                    printtitle(bookLibrary, isbnLibrary, statusLibrary, i);
                }
            }
        }
        // prints all invalid library entries
        else if (command[0] == 'i' || command[0] == 'I') {
            for (size_t i = 0; i < bookLibrary.size(); i++) {
                if (!validentry(isbnLibrary, statusLibrary, i)) {
                    invalidLibrary.push_back(i);
                }
            }
            cout << "The Following Library Entries Have Invalid Data\n-----------------------------------------------\n";
            if (bookLibrary.size() == 0) {
                cout << "The library has no books.\n";
            }
            else {
                if (invalidLibrary.size() == 0) {
                    cout << "The library has no invalid entries.\n";
                }
                else {
                    for (size_t i = 0; i < invalidLibrary.size(); i++) {
                        int temp = invalidLibrary.at(i);
                        cout << bookLibrary.at(temp) << " --- " << isbnLibrary.at(temp) << " --- " << getStatus(statusLibrary, temp) << endl;
                    }
                }
            }
        }
        // load library from file
        else if (command[0] == 'l' || command[0] == 'L') {
            cout << "What database to read from? ";
            getline(cin, command);
            loadFromFile(bookLibrary, isbnLibrary, statusLibrary, command);
        }
        // export library to file
        else if (command[0] == 'o' || command[0] == 'O') {
            cout << "What database to output to? ";
            getline(cin, command);
            cout << endl;
            exportlibrary(bookLibrary, isbnLibrary, statusLibrary, command);
        }
        // prints library's current stats
        else if (command[0] == 'p' || command[0] == 'P') {
            printcheckoutstatus(statusLibrary);
        }
        // removes book by title or isbn
        else if (command[0] == 'r' || command[0] == 'R') {
            cout << "Would you like remove by (1) name or (2) ISBN.\nEnter the numeric choice: ";
            getline(cin, command);
            cout << endl;
            int tempi = 0;
            if (stoi(command) == 1) {
                cout << "Enter the book name: \n";
                getline(cin, command);
                tempi = getBookIndex(bookLibrary, command);
                cout << "The Following Entry Was Removed From The Library\n------------------------------------------------\n";
                removebook(bookLibrary, isbnLibrary, statusLibrary, tempi);
            }
            else if (stoi(command) == 2) {
                cout << "Enter the book 13-digit ISBN (with dashes): \n";
                getline(cin, command);
                tempi = getISBNindex(isbnLibrary, command);
                cout << "The Following Entry Was Removed From The Library\n------------------------------------------------\n";
                removebook(bookLibrary, isbnLibrary, statusLibrary, tempi);
            }
            else {
                cout << "Invalid remove by choice option.\n";
            }
        }
        // search by title or isbn
        else if (command[0] == 's' || command[0] == 'S') {
            cout << "Would you like to search by (1) name or (2) ISBN.\nEnter the numeric choice: ";
            getline(cin, command);
            cout << endl;
            int tempi = 0;;
            if (stoi(command) == 1) {
                cout << "Enter the book name: \n";
                getline(cin, command);
                tempi = getBookIndex(bookLibrary, command);
                cout << "The Following Are Your Search Results\n-------------------------------------\n";
                searchbook(bookLibrary, isbnLibrary, statusLibrary, tempi);
            }
            else if (stoi(command) == 2) {
                cout << "Enter the book 13-digit ISBN (with dashes): \n";
                getline(cin, command);
                tempi = getISBNindex(isbnLibrary, command);
                cout << "The Following Are Your Search Results\n-------------------------------------\n";
                searchbook(bookLibrary, isbnLibrary, statusLibrary, tempi);
            }
            else {
                cout << "Invalid search by choice option.\n";
            }
        }

        cout << endl;
    } while (!(command == "x" || command == "X"));

    return 0;
}

// Function definitions for all the glorious function decomposition

/// Adds parameters title isbn status into respective library vectors
void addBook(vector<string> &bookLibrary, vector<string> &isbnLibrary, vector<int> &statusLibrary, const string title, const string isbn, const int status) {
    if (!validtitle(title)) {
        cout << "The book title cannot contain commas.\n";
    }
    else if (validisbn(isbn) && validtitle(title)) {
        bookLibrary.push_back(title);
        isbnLibrary.push_back(isbn);
        statusLibrary.push_back(status);
    }
    else if (!validisbn(isbn)) {
        bookLibrary.push_back(title);
        isbnLibrary.push_back(isbn);
        statusLibrary.push_back(status);
    }
}

/// exports library to filename
void exportlibrary(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const string filename) {
    string currtitle, currisbn;
    int currstatus;
    ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        cout << "Could not open database file for writing.\n";
    }
    for (size_t i = 0; i < bookLibrary.size(); i++) {
        currtitle = bookLibrary.at(i);
        currisbn = isbnLibrary.at(i);
        currstatus = statusLibrary.at(i);

        file << currtitle << ", " << currisbn << ", " << currstatus << endl;
    }
    file.close();
    cout << "Successfully exported library to " << filename << endl;
}

/// returns index of book search based on title, returns -1 if not found
int getBookIndex(vector<string>& bookLibrary, const string title) {
    for (int i = bookLibrary.size()-1; i >= 0; i--) {
        if (bookLibrary.at(i) == title) {
            return i;
        }
    }
    return -1;
}

/// returns index of isbn search based on isbn, returns -1 if not found
int getISBNindex(vector<string>& isbnLibrary, const string isbn) {
    for (int i = isbnLibrary.size() - 1; i >=0; i--) {
        if (isbnLibrary.at(i) == isbn) {
            return i;
        }
    }
    return -1;
}

/// returns status of book
string getStatus(vector<int>& statusLibrary, const int index) {
    if (statusLibrary.at(index) == 0) {
        return "In Library";
    }
    else if (statusLibrary.at(index) == 1) {
        return "Checked Out";
    }
    else if (statusLibrary.at(index) == 2) {
        return "On Loan";
    }
    else if (statusLibrary.at(index) == 3) {
        return "Unknown State";
    }
    else {
        return "Invalid State";
    }
}

/// loads library from file
void loadFromFile(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const string filename) {
    string line;
    string currtitle;
    string currisbn;
    string tempstatus;
    int currstatus;
    int linesread = 0;

    ifstream file;
    file.open(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            parseBook(line, currtitle, currisbn, currstatus);
            addBook(bookLibrary, isbnLibrary, statusLibrary, currtitle, currisbn, currstatus);
            linesread++;
            currtitle = "";
            currisbn = "";
            currstatus = 0;
        }
        file.close();
        cout << "Read in " << linesread << " lines from the file.\n";
    }
    else {
        cout << "Could not find the database file.\n";
    }
}

/// parses bookisbn into currtitle and currisbn and currstatus;
void parseBook(const string bookisbn, string& currtitle, string& currisbn, int& currstatus) {
    int comma1 = bookisbn.find(',');
    int comma2 = bookisbn.find_last_of(',');

    if ((comma1 != comma2) && (comma1 != -1) && (comma2 != -1)) {
        int isbnlength = comma2 - comma1;
        currtitle = bookisbn.substr(0, comma1);
        currisbn = bookisbn.substr(comma1 + 2, isbnlength - 2);
        currstatus = stoi(bookisbn.substr(comma2 + 1));
    }
    else if ((comma1 != -1) && (comma2 != -1) && (comma1 == comma2)) { //1 comma found, title and isbn found
        currtitle = bookisbn.substr(0, comma1);
        currisbn = bookisbn.substr(comma1 + 2);
        currstatus = 0;
    }
    else if ((comma1 == -1) && (comma2 == -1)) { //no commas found, title found
        currtitle = bookisbn;
        currisbn = "000-0-00-000000-0";
        currstatus = 0;
    }
}

/// print out the checkout status from the library
void printcheckoutstatus(vector<int> statusLibrary) {
    int inLibrary = 0;
    int checkedOut = 0;
    int onLoan = 0;
    int unknownstatus = 0;
    int otherstatus = 0;
    for (int curr : statusLibrary) {
        if (curr == 0) {
            inLibrary++;
        }
        else if (curr == 1) {
            checkedOut++;
        }
        else if (curr == 2) {
            onLoan++;
        }
        else if (curr == 3) {
            unknownstatus++;
        }
        else {
            otherstatus++;
        }
    }
    cout << "Your Current Library's Stats\n----------------------------\n";
    cout << "Total Books: " << statusLibrary.size() << endl;
    cout << "   In Library: " << inLibrary << endl;
    cout << "   Checked Out: " << checkedOut << endl;
    cout << "   On Loan: " << onLoan << endl;
    cout << "   Unknown: " << unknownstatus << endl;
    cout << "   Other: " << otherstatus << endl;
}

/// @brief print out the main menu of the management system
void printMenu()
{
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;    
    cout << "I - List Invalid Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program" << endl;
}

/// prints out title isbn status at index in vectors
void printtitle(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index) {
    cout << bookLibrary.at(index) << " --- " << isbnLibrary.at(index) << " --- " << getStatus(statusLibrary, index) << endl;
}

/// removes book from index;
void removebook(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index) {
    if (index == -1) {
        cout << "No matching entry found in the library.\n";
    }
    else {
        printtitle(bookLibrary, isbnLibrary, statusLibrary, index);
        bookLibrary.erase(bookLibrary.begin() + index);
        isbnLibrary.erase(isbnLibrary.begin() + index);
        statusLibrary.erase(statusLibrary.begin() + index);
    }
}

///  searched for book by title or isbn and prints out matching books in library
void searchbook(vector<string>& bookLibrary, vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index) {
    if (index == -1) {
        cout << "No matching entry found in the library.\n";
    }
    else {
        printtitle(bookLibrary, isbnLibrary, statusLibrary, index);
    }
}

/// checks if valid entry, checks isbn and status
bool validentry(vector<string>& isbnLibrary, vector<int>& statusLibrary, const int index) {
    if (validisbn(isbnLibrary.at(index)) && validstatus(statusLibrary, index)) {//checks if valid isbn and status
        return true;
    }
    else {
        return false;
    }
}

/// checks if valid isbn format, returns true or false
bool validisbn(string isbn) {
    if (isbn.length() != 17) {
        return false;
    }
    else if (isbn[3] != '-' || isbn[5] != '-' || isbn[8] != '-' || isbn[15] != '-') {
        return false;
    }
    else { //isbn is 17 length and correct format, checks isbn check digit
        int calc = 0;
        calc += isbn[0]-48 + (isbn[1] - 48) * 3 + isbn[2] - 48;
        calc += (isbn[4]-48) * 3;
        calc += isbn[6] - 48 + (isbn[7] - 48) * 3;
        calc += (isbn[9] - 48 + (isbn[10] - 48) * 3 + isbn[11] - 48 + (isbn[12] - 48) * 3 + isbn[13] - 48 + (isbn[14] - 48) * 3);
        calc = calc % 10;
        calc = 10 - calc;
        if (calc == isbn[16]-48) {
            return true;
        }
        else {
            return false;
        }
    }
}

/// checks if valid status
bool validstatus(vector<int> statusLibrary, const int index) {
    if (statusLibrary.at(index) < 0 || statusLibrary.at(index) > 3) {
        return false;
    }
    else {
        return true;
    }
}

/// checks if valid title, returns true or false
bool validtitle(const string title) {
    for (size_t i = 0; i < title.length(); i++) {
        if (title[i] == ',') {
            return false;
        }
    }
    return true;
}