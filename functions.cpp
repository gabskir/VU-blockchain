#include "functions.h"
#include "sha256.h"

void printUsage() {
    cout << "************* USAGE *************" << endl
         << endl
         << "Perform hashing with read file: " << endl
         << "<program> file <filename>" << endl
         << endl
         << "Perform hashing with manual input: " << endl
         << "<program> manual" << endl
         << endl
         << "Generate files of wanted length: " << endl
         << "<program> generate <length>" << endl
         << endl
         << "Perform avalanche effect test: " << endl
         << "<program> avalanche test" << endl
         << endl
         <<"Perform collision test: " << endl
         << "<program> collision test" << endl
         << endl
         << "Perform konstitucija test: " << endl
         << "<program> konstitucija test" << endl
         << endl
         << "Perform hashing with salt: " << endl
         << "<program> salt" << endl
         << "<program> salt file <filename>" << endl
         << "<program> salt pairs" << endl
         << endl
         << "Perform puzzle friendliness test: " << endl
         << "<program> salt puzzle" << endl
         << endl
         << "Perform SHA256 vs myHash test: " << endl
         << "<program> compare" << endl;
}

//---------------------------HASHING FUNCTIONS---------------------------------

unsigned long long int leftRotate (unsigned long long int n, unsigned long long int d) {
    return (n << d) | (n >> (INT_BITS - d));
}

unsigned long long int rightRotate (unsigned long long int n, unsigned long long int d) {
    return (n >> d) | (n << (INT_BITS - d));
}

//random bit operation are performed and the result is added to the sum

unsigned long long int hashing(const string& input) {
    int lastIndex = input.length() - 1;
    unsigned long long int sum = 0xb5ad4eceda1ce2a9; 
    //unsigned long long int sum = 0x243f6a8885a308d3; 

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

//fill the bitset with random bits to reach 256 bits

void fillBits(std::bitset<256> &bits, unsigned long long int seed) {
    for (int i = 64; i < 256; i += 64) {
        seed = leftRotate(seed, 19) ^ rightRotate(seed, 29) ^ (seed * 0xb5ad4eceda1ce2a9);
        //seed = leftRotate(seed, 19) ^ rightRotate(seed, 29) ^ (seed * 0x243f6a8885a308d3);
        //cout << "Bits1 " << endl << bits << endl;

        bits = bits << 64;
        //cout << "Bits2 " << endl << bits << endl;

        std::bitset<256> block(seed);
        bits = bits ^ block;
        //cout << "Bits3 " << endl << bits << endl;
    }
}

//convert the bitset to hex string

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

//---------------------------SALT FUNCTIONS---------------------------------

//16 byte salt is generated and added to the input string
std::unordered_set<string> saltSet;
using hrClock = std::chrono::high_resolution_clock;
std::mt19937 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));

string generateSalt() {
    string salt;
    
    do {
        vector<unsigned char> saltBytes(16);
        std::uniform_int_distribution<int> dist(0, 255);

        for (int i = 0; i < 16; i++) { 
            saltBytes[i] = static_cast<unsigned char>(dist(mt));
        }

        std::stringstream ss;
        for(unsigned char byte : saltBytes) {
            ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
        }
        
        salt = ss.str();
    } while (saltSet.find(salt) != saltSet.end());
    
    saltSet.insert(salt);

    return salt;
}

std::pair<string, string> hashWithSalt(const string& input, bool writeToFile) {
    string salt = generateSalt();
    string saltedInput = salt + input;
    string hashedValue = getHashString(saltedInput);

    if(writeToFile) {
        std::ofstream outFile("hashes_and_salts.txt", std::ios_base::app);
        if(outFile.is_open()) {
            outFile << "Hash: " << hashedValue << "\n";
            outFile << "Salt: " << salt << "\n";
            outFile << "------------------\n";
            outFile.close();
        } else {
            cout << "Unable to open file for writing!" << std::endl;
        }
    }
    return std::make_pair(hashedValue, salt);
}

void puzzleFriendlinessTest () {
    string targetHash = "1df38d057fe2b25767ffbf30521be77c921c47da36c56bf6ed3ffed312a01874";
    int maxAttempts = 1000000;
    int attempts = 0;
    bool found = false;

    for (int i = 0; i < maxAttempts; i++) {
        string randomInput = generateString(10);
        string salt = generateSalt();
        string hashOutput = getHashString(salt + randomInput);
        
        attempts++;
        
        if (hashOutput == targetHash) {
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Found an input producing the target hash after " << attempts << " attempts." << endl;
    } else {
        cout << "After " << attempts << " attempts, no input was found to produce the target hash." << endl;
    }
}

//---------------------------FILE FUNCTIONS---------------------------------

//generate 2 files with 1 symbol and 4 files with random strings of given length
void generateFile(int length) {

    for (int i = 1; i <= 2; i++) {
        std::ofstream file("file_symbol" + std::to_string(i) + ".txt");
        file << generateString(1);
        file.close();
    }

    for (int i = 1; i <= 2; i++) {
        std::ofstream file("file_str1_" + std::to_string(i) + ".txt");
        file << generateString(length);
        file.close();
    }

    if (length > 0) {
        string str = generateString(length);
        char middleChar1 = generateString(1)[0];
        char middleChar2 = generateString(1)[0];
        while (middleChar2 == middleChar1) {
            middleChar2 = generateString(1)[0];
        }
        str[length / 2] = middleChar1;
        std::ofstream file1("file_str2_1.txt");
        file1 << str;
        file1.close();

        str[length / 2] = middleChar2;
        std::ofstream file2("file_str2_2.txt");
        file2 << str;
        file2.close();
    }
}

string readFile(const string &fileName){ 
    std::ifstream in(fileName);

    if (!in.is_open()) {
        cout << "Error: File " << fileName << " could not be opened.";
    } else {
        cout << "\nReading file..." << endl;
    }
        std::stringstream buffer;
        buffer << in.rdbuf();

        string fileContent = buffer.str();
        cout << fileContent << endl;

        return fileContent;  

}

using hrClock = std::chrono::high_resolution_clock;
std::mt19937 tm(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));

string generateString(int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        
    string result;
    result.reserve(length);

    std::uniform_int_distribution<int> dist(0, sizeof(alphanum) - 2);


    for (int i = 0; i < length; ++i)
        result += alphanum[dist(tm)];

    return result;
}


//---------------------------TEST FUNCTIONS---------------------------------
void collisionTest () {

    vector<int> lengths = {10, 100, 500, 1000};
    int distCollisionCount[4] = {0};
    int collisionCount = 0;

    for(int i = 0; i < 4; ++i) {

        int length = lengths.at(i);

        for(int j = 0; j < 25000; ++j) {
            string str1 = generateString(length);
            string str2 = generateString(length);
            
            while (str1 == str2) {
                str1 = generateString(length);
                str2 = generateString(length);
            }

            string hash1 = getHashString(str1);
            string hash2 = getHashString(str2);

            if (hash1 == hash2) {
                distCollisionCount[i]++;
            }
        }
        cout << "Number of collisions for length " << length << ": " << distCollisionCount[i] << endl;
        collisionCount += distCollisionCount[i];
    }

    
    cout << "Number of collisions found: " << collisionCount << std::endl;
    
}

void konstitucijosTest() {

    std::ifstream file("konstitucija.txt");
    if(!file.is_open()) {
        cout << "Unable to open file konstitucija.txt" << endl;
        exit(0);
    }

    vector<string> lines;
    string line;
    while(getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    // std::ofstream outfile("hashes.txt");
    // if(!outfile.is_open()) {
    //     cout << "Unable to open file hashes.txt" << std::endl;
    //     exit(0);
    // }

    Timer timer;
    for(int i = 1; i <= lines.size(); i *= 2) {
        int iterations = 5;
        double totalDuration = 0.0;
        
        for(int j = 0; j < iterations; ++j) {
            string toHash;
            for(int k = 0; k < i && k < lines.size(); ++k) {
                toHash += lines[k];
            }
            timer.reset();
            string hashResult = getHashString(toHash);
            totalDuration += timer.elapsed();
            //outfile << "Hashing " << i << " line(s) together: " << hashResult << std::endl;
        }
        
        double averageDuration = totalDuration / iterations;
        cout << "Average time to hash " << i << " line(s) together: " << std::fixed << std::setprecision(6) <<  averageDuration << "s" << endl;
        // outfile << "Average time to hash " << i << " line(s) together: " << std::fixed << std::setprecision(6) <<  averageDuration << "s" << endl;
        // outfile << "----------" << endl;
    }

    //outfile.close();
    lines.clear();
}

std::bitset<256> hexStringToBitset(const string& hexStr) {
    std::bitset<256> bits;
    for(size_t i = 0; i < hexStr.size(); ++i) {
        unsigned long long val = std::stoull(hexStr.substr(i, 1), nullptr, 16);
        for(size_t j = 0; j < 4; ++j)
            bits[i * 4 + (3 - j)] = (val >> j) & 1;
    }
    return bits;
}
// void avalancheTest() {
//     const std::vector<int> lengths = {10, 100, 500, 1000};
    
//     for (const auto& length : lengths) {
//         float minBit = 100, maxBit = 0, avgBit = 0;
//         float minHex = 100, maxHex = 0, avgHex = 0;
        
//         for (int i = 0; i < 25000; ++i) {
//             std::string str1 = generateString(length);
//             std::string str2 = str1; // Copy the string
            
//             // Change the first character of the second string in the pair
//             str2[0] = str2[0] == 'a' ? 'b' : 'a';
            
//             auto hash1 = getHashString(str1);
//             auto hash2 = getHashString(str2);

//             auto bits1 = hexStringToBitset(hash1);
//             auto bits2 = hexStringToBitset(hash2);
            
//             // For Hex
//             size_t hexCount = 0;
//             for (size_t j = 0; j < hash1.length(); ++j)
//                 if(hash1[j] != hash2[j])
//                     ++hexCount;
//             float hexDiff = (hexCount * 100.0) / 64;
//             minHex = std::min(minHex, hexDiff);
//             maxHex = std::max(maxHex, hexDiff);
//             avgHex += hexDiff;

//             // For Bit
//             auto xorResult = bits1 ^ bits2;
//             float bitCount = xorResult.count();
//             float bitDiff = (bitCount * 100.0) / 256;
//             minBit = std::min(minBit, bitDiff);
//             maxBit = std::max(maxBit, bitDiff);
//             avgBit += bitDiff;
//         }
        
//         avgBit /= 25000;
//         avgHex /= 25000;
        
//         cout << "For strings of length " << length << ":\n";
//         cout << "Hex Difference:" << 
//         endl << "- Min: " << minHex << 
//         endl << "- Max: " << maxHex <<
//         endl << "- Avg: " << avgHex << endl << endl;
//         cout << "Bit Difference: " <<
//         endl << "- Min: " << minBit << 
//         endl << "- Max: " << maxBit << 
//         endl << "- Avg: " << avgBit << endl << endl;
//         cout << "----------" << endl;
//     }
//}

void avalancheTest() {
    std::ofstream file("avalanche_test_output.txt");

    const vector<int> lengths = {10, 100, 500, 1000};
    
    for (const auto& length : lengths) {
        float minBit = 100, maxBit = 0, avgBit = 0;
        float minHex = 100, maxHex = 0, avgHex = 0;
        
        for (int i = 0; i < 25000; ++i) {
            string str1 = generateString(length);
            string str2 = str1;
            
            str2[0] = str2[0] == 'a' ? 'b' : 'a';

            file << "String 1: " << str1 << "\n";
            file << "String 2: " << str2 << "\n";
            
            auto hash1 = getHashString(str1);
            auto hash2 = getHashString(str2);

            file << "Hash 1: " << hash1 << "\n";
            file << "Hash 2: " << hash2 << "\n";

            auto bits1 = hexStringToBitset(hash1);
            auto bits2 = hexStringToBitset(hash2);
            
            size_t hexCount = 0;
            for (size_t j = 0; j < hash1.length(); ++j)
                if(hash1[j] != hash2[j])
                    ++hexCount;
            file << "Hex Count: " << hexCount << "\n";
            float hexDiff = (hexCount * 100.0) / 64;
            minHex = std::min(minHex, hexDiff);
            maxHex = std::max(maxHex, hexDiff);
            avgHex += hexDiff;

            auto xorResult = bits1 ^ bits2;
            float bitCount = xorResult.count();
            file << "Bit Count: " << bitCount << "\n";
            float bitDiff = (bitCount * 100.0) / 256;
            minBit = std::min(minBit, bitDiff);
            maxBit = std::max(maxBit, bitDiff);
            avgBit += bitDiff;
        }
        
        avgBit /= 25000;
        avgHex /= 25000;
        
        cout << "For strings of length " << length << ":\n";
        cout << "Hex Difference:" << 
        endl << "- Min: " << minHex << 
        endl << "- Max: " << maxHex <<
        endl << "- Avg: " << avgHex << endl << endl;
        cout << "Bit Difference: " <<
        endl << "- Min: " << minBit << 
        endl << "- Max: " << maxBit << 
        endl << "- Avg: " << avgBit << endl << endl;
        cout << "----------" << endl;

        file << "For strings of length " << length << ":\n";
        file << "Hex Difference:" << endl; 
        file << "- Min: " << minHex << endl; 
        file << "- Max: " << maxHex << endl;
        file << "- Avg: " << avgHex << endl << endl;
        file << "Bit Difference:" << endl; 
        file << "- Min: " << minBit << endl; 
        file << "- Max: " << maxBit << endl;
        file << "- Avg: " << avgBit << endl << endl;
        file << "----------" << endl;
    }

    file.close();
}

void sha256VSmyHash () {
    vector<int> lengths = {25000, 50000, 75000, 100000};
    const int numIterations = 10;
    
    for (int len : lengths) {
        string data = generateString(len);
        
        double totalDuration1 = 0;
        double totalDuration2 = 0;

        Timer timer1, timer2;

        for(int k=0; k<numIterations; k++) {

            timer1.reset();
            for(int i=0; i<1000; i++) {
                getHashString(data);
            }
            totalDuration1 += timer1.elapsed();

            timer2.reset();
            for(int i=0; i<1000; i++) {
                sha256(data);
            }
            totalDuration2 += timer2.elapsed();
        }

        double avgDuration1 = totalDuration1 / numIterations;
        double avgDuration2 = totalDuration2 / numIterations;

        cout << "Data Length: " << len << endl;
        cout << "Your Hash Function Average Time: " << avgDuration1 << " seconds" << endl;
        cout << "SHA256 Average Time: " << avgDuration2 << " seconds" << endl;
        cout << "-------------------------------------" << endl;
    }
}

