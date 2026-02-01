#include "seal.hpp"
#include <vector>
#include <cstdint>
#include <cstring>

/*
 * ISO‑16 Reference SHA3‑256 Implementation
 * ---------------------------------------
 * This is a compact, deterministic implementation of SHA3‑256
 * suitable for reference and audit purposes.
 *
 * It is NOT optimized for performance.
 * It is designed for clarity and correctness.
 *
 * The function provided here satisfies the requirement in seal.hpp:
 *
 *     std::vector<uint8_t> sha3_256(const uint8_t* data, size_t len);
 *
 * Hardware teams may replace this file with a hardware SHA3 block.
 */

namespace iso16 {

// ------------------------------------------------------------
// Keccak-f[1600] constants
// ------------------------------------------------------------

static const uint64_t KECCAKF_RNDC[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

static const int KECCAKF_ROTC[24] = {
     1,  3,  6, 10, 15, 21,
    28, 36, 45, 55,  2, 14,
    27, 41, 56,  8, 25, 43,
    62, 18, 39, 61, 20, 44
};

static const int KECCAKF_PILN[24] = {
    10,  7, 11, 17, 18, 3,
     5, 16,  8, 21, 24, 4,
    15, 23, 19, 13, 12, 2,
    20, 14, 22,  9,  6,  1
};

// ------------------------------------------------------------
// Rotate left
// ------------------------------------------------------------
static inline uint64_t rol(uint64_t x, int s) {
    return (x << s) | (x >> (64 - s));
}

// ------------------------------------------------------------
// Keccak-f[1600] permutation
// ------------------------------------------------------------
static void keccakf(uint64_t st[25]) {
    for (int round = 0; round < 24; round++) {
        uint64_t bc[5];

        // Theta
        for (int i = 0; i < 5; i++)
            bc[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];

        for (int i = 0; i < 5; i++) {
            uint64_t t = bc[(i + 4) % 5] ^ rol(bc[(i + 1) % 5], 1);
            for (int j = 0; j < 25; j += 5)
                st[j + i] ^= t;
        }

        // Rho + Pi
        uint64_t t = st[1];
        for (int i = 0; i < 24; i++) {
            int j = KECCAKF_PILN[i];
            uint64_t tmp = st[j];
            st[j] = rol(t, KECCAKF_ROTC[i]);
            t = tmp;
        }

        // Chi
        for (int j = 0; j < 25; j += 5) {
            uint64_t row[5];
            for (int i = 0; i < 5; i++)
                row[i] = st[j + i];
            for (int i = 0; i < 5; i++)
                st[j + i] ^= (~row[(i + 1) % 5]) & row[(i + 2) % 5];
        }

        // Iota
        st[0] ^= KECCAKF_RNDC[round];
    }
}

// ------------------------------------------------------------
// SHA3‑256 absorb + squeeze
// ------------------------------------------------------------
std::vector<uint8_t> sha3_256(const uint8_t* data, size_t len) {
    const size_t rate = 136; // SHA3‑256 rate in bytes
    uint64_t st[25];
    std::memset(st, 0, sizeof(st));

    // Absorb
    size_t offset = 0;
    while (len - offset >= rate) {
        for (size_t i = 0; i < rate / 8; i++) {
            uint64_t v;
            std::memcpy(&v, data + offset + i * 8, 8);
            st[i] ^= v;
        }
        keccakf(st);
        offset += rate;
    }

    // Final block
    uint8_t temp[rate];
    std::memset(temp, 0, rate);
    size_t remaining = len - offset;
    std::memcpy(temp, data + offset, remaining);

    // Padding: SHA3 domain separation
    temp[remaining] = 0x06;
    temp[rate - 1] |= 0x80;

    for (size_t i = 0; i < rate / 8; i++) {
        uint64_t v;
        std::memcpy(&v, temp + i * 8, 8);
        st[i] ^= v;
    }

    keccakf(st);

    // Squeeze 32 bytes
    std::vector<uint8_t> out(32);
    std::memcpy(out.data(), st, 32);
    return out;
}

} // namespace iso16
