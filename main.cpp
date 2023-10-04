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

        int length = std::stoi(argv[2]);
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
    } else if (command == "salt" && argv[2] == NULL) {

        string input;
        cout << "Type your text: ";
        getline(cin, input);
        auto result = hashWithSalt(input, false);
        cout << "Hash: " << result.first << endl;
        cout << "Salt: " << result.second << endl;

    } else if (command == "salt" && strcmp(argv[2], "file") == 0) {

        string filename = argv[3];
        string input = readFile(filename);
        auto result = hashWithSalt(input, false);
        cout << "Hash: " << result.first << endl;
        cout << "Salt: " << result.second << endl;

    } else if (command == "salt" && strcmp(argv[2], "pair") == 0) {

        std::ofstream clearFile("hashes_and_salts.txt");
        clearFile.close();
        vector<int> lengths = {10, 100, 500, 1000};
        int distCollisionCount[4] = {0};
        int distCollisionCountSalt[4] = {0};
        int collisionCount = 0;
        int collisionCountSalt = 0;

        for(int i = 0; i < 4; ++i) {

            int length = lengths.at(i);

            for(int j = 0; j < 25000; ++j) {
                string str1 = generateString(length);
                string str2 = str1;

                auto hash1 = hashWithSalt(str1, true);
                auto hash2 = hashWithSalt(str2, true);

                if (hash1.first == hash2.first) {
                    distCollisionCount[i]++;
                }
                if (hash1.second == hash2.second) {
                    distCollisionCountSalt[i]++;
                }
            }
            cout << "Number of collisions for length " << length << ": " << distCollisionCount[i] << endl;
            collisionCount += distCollisionCount[i];
        
            cout << "Number of salt collisions for length " << length << ": " << distCollisionCountSalt[i] << endl;
            collisionCountSalt += distCollisionCountSalt[i];
        }  
        cout << "Number of collisions found: " << collisionCount << endl;
        cout << "Number of salt collisions found: " << collisionCountSalt << endl;

    } else if (command == "salt" && strcmp(argv[2], "puzzle") == 0) {

        puzzleFriendlinessTest();

    } else if (command == "compare"){
        sha256VSmyHash();
    } else {
        cout << "Invalid command" << endl;
        printUsage();
        return 0;
    }
    
    return 0;
}
