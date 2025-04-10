
# AERIS-256

A lightweight block cipher inspired by AES and Simon-Speck ciphers

## Features

- Block size: 64 bits
- Key size: 256 bits
- Feistel Network
- Keys derived from SHA256 hash of the password and salt
- Mode of operation: ECB
- Written in C with minimal dependencies (OpenSSL for SHA256)

## Installation and Usage

Clone the repository and compile the source files

```bash
git clone https://github.com/xshthkr/aeris-256.git
cd aeris-256
make
```

The binary file will be in the `bin` directory.

To encrypt a file:

```bash
./cipher <filepath> enc <password>
```

To decrypt a file:

```bash
./cipher <filepath> dec <password>
```

The file paths should be relative to the binary executable. The binary can be added to `path` environment variables to be executed from any directory.

## How it works

A `uint8_t` array buffer stores the raw bytes of the input file or string. This array is padded to a length that is an integral multiple of the block size in bytes. This lets us access each individual blocks of the array by typecasting it to `uint4_t` array. Each block of the plaintext array (64 bits) is encrypted independently (ECB mode) and written to another array. The Feistel Network goes through several rounds of operations to encrypt each block. Each block is split into `uint32_t` (32 bit) halves which are updated using a round function `f()`, `XOR` operations, and a rotating set of keys. A 256 bit key is derived from hashing `password + salt` using SHA256. The round keys are generated from this master key by rotating and selecting 8-byte chunks. Currently, each block is encrypted independently (ECB mode). CBC or CTR mode should be implemented in the near future.

## References

- NSA paper: [The Simon and Speck Families of Lightweight Block Ciphers](https://eprint.iacr.org/2013/404.pdf)
- NIST paper: [Advanced Encryption Standard (AES)](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf)
