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
