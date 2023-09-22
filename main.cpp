#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <filesystem> 
#include <vector>
#include <string>
#include <fstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void readFile(){    
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
            break;
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
        readFile();
    }
    else {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        string input;
        cout << "Type your text: ";
        getline (cin, input);

    }
}