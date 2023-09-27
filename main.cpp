#include "functions.h"

int main(int argc, char *argv[]) {

    if (argc == 1) {
        printUsage();
        return 0;
    }

    string command = argv[1];

    if(command == "file") {
        string filename = argv[2];
        string input = readFile(filename);
        cout << "Hex value: " << getHashString(input) << endl;
    } else if(command == "manual") {
         string input;
         cout << "Type your text: ";
         getline(cin, input);
         cout << "Hex value: " << getHashString(input) << endl;
    } else if(command == "generate") {

        if(argc != 3) {
            cout << "Invalid number of arguments" << endl;
            return 0;
        }

        int length = atoi(argv[2]);
        cout << "Generating files..." << endl;
        generateFile(length);
        cout << "Files generated." << endl << "Available files: " << endl;

        string dirPath = "./";
        vector<string> files;
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                files.push_back(entry.path().filename().string());
            }
        }

        for (int i = 0; i < files.size(); ++i) {
            cout << i + 1 << ". " << files[i] << endl;
        }

        files.clear();

    } else if ( command == "collision" ) {
        collisionTest();
    } else if ( command == "konstitucija" ) {
        konstitucijosTest();
    } else if ( command == "avalanche" ) {
        avalancheTest();
    } else {
        cout << "Invalid command" << endl;
        printUsage();
        return 0;
    }
    
    return 0;
}
