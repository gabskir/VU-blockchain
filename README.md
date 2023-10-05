# VU-blockchain
## Hash generator
A hash generator developed according to the requirements of the first task of the "Blockchain Technology" module at Vilnius University.

### Requirements:
1. The input to the hash function can be a character string of any length.
2. The output of the hash function must always be the result of the same fixed size (preferably 256 bits long, i.e. a 64-character hex).
3. The hash function should be deterministic, i.e. for the same input the output should always be the same.
4. The hash for any input value should be computed quickly - efficiently.
5. It should be practically impossible to reproduce the original input from the hash function output.
6. The hash function should be *"collision resistant"*
7. At least a minimal change to the input, e.g. replacing "Lietuva" with "lietuva", must result in a substantially different hash value, i.e. the so-called *Avalanche effect* must be satisfied.
   
--- 

### Instructions:
The behaviour of the program depends on the given command line arguments:
- to hash an existing file: **&lt;program&gt; file &lt;filename&gt;**
- to hash an manually entered string: **&lt;program&gt; manual**
- to generate files¹ of wanted length **&lt;program&gt; generate &lt;length&gt;**
- to perform collision test: **&lt;program&gt; collision test**
- to perform avalanche effect test: **&lt;program&gt; avalanche effect test**
- to perform efficiency test with file "konstitucija.txt": **&lt;program&gt; konstitucija test**<br>
<br>
  ¹ 6 files will be generated: 2 with different symbols, 2 with different strings of entered length and 2 with matching strings (that only differ by one character) of entered length

---

### Algorithm Pseudocode

```
Algorithm CalculateHash(input: String)
    Initialize sum to a constant hexadecimal value
    Let lastIndex be the length of the input - 1
    For each character in the input, do the following steps:
        Let curChar be the current character
        Calculate altChar by performing a bitwise rotation of curChar, the rotation amount is dependent on the index of curChar and the character's ASCII value.
        Calculate xorBuddy by performing another bitwise rotation on the character at position lastIndex in the input string.
        Perform bitwise XOR operation between altChar and xorBuddy to get xorResult
        Compute mult by multiplying xorResult by a complex mathematical expression involving powers and a constant value
        Update sum by performing bitwise XOR and rotations on xorResult and mult
        Update lastIndex based on the current index in the iteration
        
    Return sum

Algorithm FillBits(bits: Bitset, seed: Number)
    For i in sequence 64, 128, 192, 256 do the following steps:
        Update seed using a series of bitwise operations and multiplication by a constant hexadecimal value
        Perform left shift operation on bits by 64
        Create a block of bits from the seed
        Update bits by performing bitwise XOR operation between bits and block

Algorithm GetHashString(input: String)
    Calculate sum using CalculateHash function with input string
    Initialize bits with the calculated sum
    Fill bits using FillBits function with bits and sum
    Initialize an empty string hashString
    For each 4 bits in bits, do the following steps:
        Convert the 4 bits to a hexadecimal character
        Append the character to hashString
    
    Return hashString

```
--- 

### Testing 

#### Deterministic:

After hashing the same character, and identical strings of the same length, the result showed that one input always gives the same output. We can therefore conclude that the created hash function is deterministic. <br>
<sub> Test for the same symbol and string was performed five times, to assure, that output did not change. </sub> <br>
<br>
*Examples* <br>
<br>
Hash value of symbol 'a':
```
b1e71ab42390948da056c0e55a3d4230e1b3872131fbf290954738f9c46a87ad
```
Hash value of string "labas":

```
6cca107a4c714b01b4607ae049a6426a10f19e9ac65ff4ada3c757793bf43b68
```
This test also proves that input can be a character string of any length (different length file texts were also taken to prove this statement), but output will always be a fixed-length hex string (64 characters long). 

#### Efficiency test:
![Line graph](https://github.com/gabskir/VU-blockchain/blob/v0.1/line-graph.png?raw=true)

| Line Count | 1   | 2   | 4   | 8   | 16  | 32  | 64  | 128 | 256 | 512 |
|-------------|------|------|------|------|------|------|------|------|------|------|
| Time (ms)  |0.015|0.029|0.035|0.050|0.117|0.217|0.581|0.867|1.735|2.865|
<br>
After the following testing: hash one line from the file konstitucija.txt and measure the time it took (repeat this experiment by hashing 2 lines, 4 lines, 8 lines, etc. (16,32, 64, ...)), we can observe that the algorithm is quite efficient and hashes quickly. However, to describe the results more accurately, we would need to hash a much larger file and observe patterns. <br>

#### Collision resistance test:
To perform this test, 100 000 string pairs were generated (25 000 - ten characters long, 25 000 - one hundred characters long, 25 000 - five hundred characters long and 25 000 - one thousand characters long) and it was checked whether in all cases the resulting pairs hashes do not match. 

| String Length | 10      | 100     | 500     | 1000    |
|---------------|---------|---------|---------|---------|
| Collision     | 0       | 0       | 0       | 0       |

No collisions were found and therefore we can partially verify that the generated hash function is collision resistant. However, testing for true collision resistance requires much more sophisticated and time-consuming testing.

#### Avalanche effect test:
To perform this test, 100 000 string pairs were generated (25 000 - ten characters long, 25 000 - one hundred characters long, 25 000 - five hundred characters long and 25 000 - one thousand characters long).
The strings in the pair differed from each other only by the first character. The percentage difference between the hashes in the pair was evaluated both at bit and hex level. The average was derived by running this test five times.

| String Length | Min Hex Difference | Max Hex Difference | Avg Hex Difference | Min Bit Difference | Max Bit Difference | Avg Bit Difference |
|---------------|--------------------|--------------------|--------------------|--------------------|--------------------|--------------------|
| 10            | 73.4375            | 100                | 91.8847            | 35.5469            | 63.6719            | 48.9169            |
| 100           | 79.6875            | 100                | 93.1845            | 36.3281            | 62.1094            | 49.5338            |
| 500           | 75                 | 100                | 92.0805            | 34.375             | 60.9375            | 48.9964            |
| 1000          | 75                 | 100                | 92.4707            | 34.375             | 61.3281            | 49.1384            |

Taking into account the average percentage difference (which exceeds 90%), we can conclude that the created function does indeed represent the avalanche effect well.

### Conclusions
After evaluating the test results, we can identify the strengths and weaknesses of the developed algorithm. <br>
<br>
*Upsides*
- regardless of input length, the output is always 64 characters hex
- the same input always produces the same output
- the developed algorithm provides an excellent representation of the avalanche effect <br>
<br>

*Downsides*
- from performed tests we cannot clearly see how often collisions occur
- we can observe that hashing a large file would take a long time

## Additional tasks
### Salt generator 
A function **generteSalt()** is used to generate a 16-byte (128-bit) random salt. This is achieved using the Mersenne Twister random number engine and a uniform distribution that spans all possible byte values (0-255). Each of the 16 bytes is converted into its 2-character hexadecimal representation, resulting in a 32-character long hexadecimal string. The generated salt is then checked against a set of previously produced salts. If this particular salt has already been created, the function will regenerate a new salt. This loop continues until a unique salt is obtained. Once a unique salt is produced, it's added to the saltSet to keep track for future uniqueness checks. <br>
Then in the function **hashWithSalt** unique salt (that was generated in the previous function) is appended to the input. Combination of input and salt is then hashed using created hash generator. <br>

Simple tests to check hiding and puzzle-friendliness properties were created. <br>
#### Hiding
For each predefined string length (10, 100, 500, 1000), the function generates 25,000 pairs of identical strings. Each of these strings is passed through the **hashWithSalt()** function.
The function then checks for two things:
- If the hashes of the two strings are identical.
- If the salted hashes of the two strings are identical. <br>

The number of collisions for both cases is tracked for each string length. <br>
By generating pairs of identical strings and assessing collisions, the function allows us to see how adding salt can affect the hash outputs (even though the strings are identical, the salted hash should differ due to the random salt). The fewer collisions found with salt, the more evidence we have of the hiding property in play, especially with the use of salt.

| String Length | 10      | 100     | 500     | 1000    |
|---------------|---------|---------|---------|---------|
| Collision of hash    | 0       | 0       | 0       | 0       |
| Collision of salt    | 0       | 0       | 0       | 0       |

#### Puzzle - Friendliness
The purpose of this exercise is to test the difficulty of finding an input that hashes to a predetermined output (the targetHash). This difficulty should be non-trivial but not infeasible, in order to confirm puzzle friendliness. However, this simple test function has limitations: it uses a static target hash, meaning it's only testing for one specific output. <br>
Nevertheless, with this simple test we can see that after generating 1000000 random inputs and salts, the match for target hash was not found.

```
After 1000000 attempts, no input was found to produce the target hash.
```
--- 

### SHA256 and myHash speed comparison
The function **sha256VSmyHash** compares the performance of a custom hash function (getHashString) against the SHA-256 cryptographic hash function over varying data lengths. For each specified data length, it repeatedly hashes randomly generated data strings and calculates the average execution time for both hash functions. The results, including the average execution times and data lengths, are then printed to the console. <br>
![Comparison line graph](https://github.com/gabskir/VU-blockchain/blob/v0.2/Comparison.png?raw=true) <br>
From the line graph we can see that sha256 performs hashing approximately 6-7 times faster than myHash. 

