# LZ78 Implementation in C++

This project is an implementation of the LZ78 compression algorithm using a tree data structure in C++. LZ78 is a lossless data compression algorithm that replaces repeated occurrences of strings with references to a dictionary. The tree data structure is used to efficiently store and search for these dictionary entries.

## Getting Started

### Prerequisites

To compile and run the code, you need to have a C++ compiler installed on your system.

## Features

- Tree-based data structure for efficient storage and retrieval of dictionary entries.
- Support for compressing and decompressing files using the LZ78 algorithm.
- Command-line interface for easy interaction with the compression and decompression functionality.

## Getting Started

To get started with this project, follow these steps:

1. Clone the repository: `git clone https://github.com/DrHurel/lz78.git`
2. Navigate to the project directory: `cd lz78`
3. Build the project: `make`
4. Run the compression algorithm: `./lz78 --compress input.txt output.lz78`
5. Run the decompression algorithm: `./lz78 --decompress output.lz78 output.txt`

## Usage

The project provides two main options:

- `lz78 --compress`: Compresses a given input file using the LZ78 algorithm and saves the compressed output to a specified file.
- `lz78 --decompress`: Decompresses a given LZ78 compressed file and saves the decompressed output to a specified file.

