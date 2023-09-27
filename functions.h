#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "mylib.h"

class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    public:
        Timer() : start{std::chrono::high_resolution_clock::now()} {}
        void reset() {
        start = std::chrono::high_resolution_clock::now();
        }
        double elapsed() const {
        return std::chrono::duration<double>
        (std::chrono::high_resolution_clock::now() - start).count();
        }
}; 

void printUsage();
unsigned long long int leftRotate (unsigned long long int n, unsigned long long int d);
unsigned long long int rightRotate (unsigned long long int n, unsigned long long int d);
unsigned long long int hashing(const string& input);
void fillBits(std::bitset<256> &bits, unsigned long long int seed);
string getHashString(const string& input);
int getFileChoice(int numFiles);
string readFile(const string &fileName);
string generateString(int length);
void generateFile(int length);
void collisionTest ();
void konstitucijosTest();
void avalancheTest();
std::bitset<256> hexStringToBitset(const string& hexStr);



#endif