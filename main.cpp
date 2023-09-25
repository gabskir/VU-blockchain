#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <filesystem> 
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <bitset>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

#define INT_BITS 64

unsigned long long int leftRotate (unsigned long long int n, unsigned long long int d) {
    return (n << d) | (n >> (INT_BITS - d));
}

unsigned long long int rightRotate (unsigned long long int n, unsigned long long int d) {
    return (n >> d) | (n << (INT_BITS - d));
}

unsigned long long int hashinimas(string input) {

    int lastIndex = input.length() - 1;
    unsigned long long int sum = 0;

    for (int i = 0; i < input.length(); i++) {  

        char curChar = input.at(i);

        cout << "nepakeistas" << endl << (std::bitset<64>) curChar << endl;
        std::bitset<64> altChar = rightRotate(curChar, curChar * 73939);
        cout << "alt" << endl << altChar << endl;

        std::bitset<64> xorBuddy = input.at(lastIndex);
        cout << "su kuo" << endl << xorBuddy << endl;

        cout << "indeksai: " << i << " " << lastIndex << endl;

        std::bitset<64> xorResult = altChar ^ xorBuddy;
        cout << "rezultatas" << endl << xorResult.to_ullong() << endl;

        std::bitset<64> diff = xorResult.to_ullong() - 1672 * pow(876, i);
        cout << "rezultatas2" << endl << diff.to_ullong() << endl;

        sum += leftRotate(diff.to_ullong(), 8191);

        if (i + 1 < input.length()/2) {    
            lastIndex--;
        }
        else if (i + 1 > input.length()/2) {
            lastIndex++;
        }
        else {
            lastIndex = 0;
        }

    }

    cout << "Suma " << endl << sum << endl;
    return sum;

}

string readFile(){    
    string dirPath = "./";
    vector<string> files;
    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry.path().filename().string());
        }
    }

    cout << "\nAvailable files:" << std::endl;
    for (int i = 0; i < files.size(); ++i) {
        cout << i + 1 << ". " << files[i] << endl;
    }

    int fileChoice;
    while (true) {
        try {
            cout << "\nEnter the number of the file you want to read: ";
            if (!(cin >> fileChoice) || fileChoice < 1 || fileChoice > files.size()) {
                throw "Wrong choice.";
            }
            break;
        }
        catch (const char* error) {
            cout << error << std::endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    string fileName = files[fileChoice-1];
    std::ifstream in(fileName);
    while (true) {
    try{
        if (!in.is_open()) {
            throw "File could not be opened.";
        } else{
            cout << "\nReading file..." << endl;
            string eil;
            getline(in, eil);
            in.close();
            //break;
            return eil;
        }
    }catch (const char* error) {
        cout << error << "Try opening other file: " << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        while (true) {
            try {
                if (!(cin >> fileChoice) || fileChoice < 1 || fileChoice > files.size())
                throw "Bloga ivestis.";
                in.open(files[fileChoice-1]);
                break;
            }
            catch (const char* error) {
                cout << error << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    }
    
    files.clear();

    }

int main() {
    
    int choice;
    cout << "Choose whether you want to: " << endl << "1. Read from the file " << endl << "2. Enter text by hand " << endl << "Your choice: ";
    cin >> choice;
    while (choice != 1 && choice != 2) {
        cout << "Wrong choice. Choose between 1 and 2: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> choice;
    }

    if (choice == 1) {
        hashinimas(readFile());
    }
    else {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        string input;
        cout << "Type your text: ";
        getline(cin, input);
        hashinimas(input);

    }
}