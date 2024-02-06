# Huffman Encoding

This program implements Huffman encoding, a popular algorithm used for lossless data compression. Huffman encoding works by assigning variable-length codes to input characters, with shorter codes assigned to more frequent characters, and longer codes assigned to less frequent characters.

## Description

The program takes a user input message, calculates the frequency of each character in the message, constructs a Huffman tree based on these frequencies, and then encodes the message using the generated Huffman codes. It also provides functionality to decode the encoded message back to its original form.

## Usage

1. **Compilation**: Compile the program using a C compiler (e.g., GCC).

## Suppose the input message is "hello". The program will output:

The Encoding Chart 
h:110
e:0
l:111
o:10

Before Compression
hello -> 01101100101101101100
After Compression
hello -> 1100011111010

Space Saved: 39.39%
Decompressed Message: hello
   ```bash
   gcc huffman_encoding.c -o huffman_encoding
