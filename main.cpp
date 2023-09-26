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

unsigned long long int hashing(const string& input) {
    int lastIndex = input.length() - 1;
    //pabandyti pakeisti i nuli
    unsigned long long int sum = 0xb5ad4eceda1ce2a9; 

    for(int i = 0; i < input.length(); i++) {
        char curChar = input[i];
        //cout << "Original" << endl << (std::bitset<64>) curChar << endl;
        
        std::bitset<64> altChar = rightRotate(curChar, (curChar + i) % INT_BITS);
        //cout << "Alt" << endl << altChar << endl;
        
        std::bitset<64> xorBuddy = leftRotate(input[lastIndex], (curChar + lastIndex) % INT_BITS);
        //cout << "XOR Buddy" << endl << xorBuddy << endl;
        
        //cout << "Indices: " << i << " " << lastIndex << endl;
        
        std::bitset<64> xorResult = altChar ^ xorBuddy;
        //cout << "XOR Result" << endl << xorResult << endl;
        
        unsigned long long int mult = xorResult.to_ullong() * (1672 * static_cast<unsigned long long int>(pow(876, i % 10 + 1)) + 1);
        //cout << "Mult" << endl << mult << endl;
        
        sum += leftRotate(xorResult.to_ullong(), 13) ^ rightRotate(mult, 7);
        //cout << "Sum" << endl << sum << endl;
        
        if(i < input.length() / 2)
            lastIndex--;
        else if(i > input.length() / 2)
            lastIndex++;
        else
            lastIndex = 0;
    }

    return sum;
}


void fillBits(std::bitset<256> &bits, unsigned long long int seed) {
    for (int i = 64; i < 256; i += 64) {
        seed = leftRotate(seed, 19) ^ rightRotate(seed, 29) ^ (seed * 0xb5ad4eceda1ce2a9);
        //cout << "Bits1 " << endl << bits << endl;

        bits = bits << 64;
        //cout << "Bits2 " << endl << bits << endl;

        std::bitset<256> block(seed);
        bits = bits ^ block;
        //cout << "Bits3 " << endl << bits << endl;
    }
}

string getHashString(const string& input) {
    unsigned long long int sum = hashing(input);
    //cout << "Suma " << endl << sum << endl;
    std::bitset<256> bits(sum);
    //cout << "Bits " << endl << bits << endl;
    fillBits(bits, sum);
    
    std::stringstream ss;
    for(int i = 0; i < 256; i += 4) {
        int val = bits[i] * 8 + bits[i + 1] * 4 + bits[i + 2] * 2 + bits[i + 3];
        ss << std::hex << val;
    }

    return ss.str();
}

int getFileChoice(int numFiles) {
    int fileChoice;
    while (true) {
        try {
            cout << "Enter file number: ";
            if (!(cin >> fileChoice) || fileChoice < 1 || fileChoice > numFiles)
                throw "Wrong choice.";
            //in.open(files[fileChoice-1]);
            break;
        } catch (const char* error) {
            cout << error << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return fileChoice;
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
    //gaunam file choice
    int fileChoice = getFileChoice(files.size());
    string fileName = files[fileChoice - 1];
    std::ifstream in(fileName);

    
    while (true) {
        try{
            if (!in.is_open()) {
                throw "File could not be opened.";
            } else{
                cout << "\nReading file..." << endl;
                break;
            }
        }catch (const char* error) {
            cout << error << "Try opening other file." << endl;
            getFileChoice(files.size());
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
    }

    // while (!in.eof()) {
        std::stringstream buffer;
        buffer << in.rdbuf();

        string fileContent = buffer.str();
        cout << fileContent << std::endl;
    // }

    return fileContent;
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
        string input = readFile();
        cout << "Hex value: " << getHashString(input) << endl;
    }
    else {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        string input;
        cout << "Type your text: ";
        getline(cin, input);
        cout << "Hex value: " << getHashString(input) << endl;


    }
}