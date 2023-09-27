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
