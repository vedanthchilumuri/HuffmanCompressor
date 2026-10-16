# Huffman Compressor

A C++ implementation of Huffman coding, a lossless compression algorithm that gives frequent characters short bit codes and rare characters long ones.

## Features

- Builds a Huffman tree from character frequencies
- Generates a prefix-free code table
- Encodes text into a bitstring and decodes it back
- Verifies that decoding reproduces the original text exactly
- Handles input containing only one distinct character

## How it works

Normal text uses 8 bits per character. Huffman coding reduces the total by matching code length to frequency.

1. **Count frequencies** of every character in the input.
2. **Build the tree** with a min-priority queue. Start with one leaf per character, weighted by its frequency. Repeatedly remove the two lowest-weight nodes, join them under a new parent whose weight is their sum, and push the parent back. When one node remains, it is the root. Rare characters end up deep in the tree and common ones stay near the top.
3. **Read off the codes.** Walk from the root to each leaf, writing `0` for a left turn and `1` for a right turn.
4. **Encode** by replacing each character with its code. No code is a prefix of another, so decoding is unambiguous.
5. **Decode** by walking the tree bit by bit from the root. Each time you reach a leaf, output its character and return to the root.

## Build and run

```bash
g++ -std=c++17 -O2 huffman_compressor.cpp -o huffman
./huffman
```

## Sample result

The demo compresses an 86-character sentence:

```
original size:   688 bits (86 bytes)
compressed size: 373 bits (47 bytes)
compression:     54.2% of original size
decoded matches original: YES
```

The most frequent character, the space, gets the 2-bit code `01`, while rare letters like `z` get 7-bit codes. The exact codes can differ between compilers because ties in the priority queue are broken arbitrarily, but the total compressed size does not change.

## Complexity

With `n` input characters and `C` distinct characters:

- Counting frequencies: O(n)
- Building the tree: O(C log C)
- Encoding and decoding: O(n)

Space is O(C) for the tree and code table.

## Limitations

- **The compressed size excludes the code table.** A real compressed file must store the tree or frequency table so a decompressor can rebuild it, which reduces the savings, especially on small inputs.
- The encoded output is kept as a string of `'0'` and `'1'` characters in memory. It is not packed into actual bytes, so it is not written to a file.
- The input text is hard-coded in `main`.
- Tree nodes are never freed.

## Future work

- Pack the bitstream into bytes and write it to a binary file
- Serialize a header with the frequency table so files can be decompressed independently
- Add a command-line interface (`compress` and `decompress` for any file)
- Free the tree and test on larger files, including binary data
