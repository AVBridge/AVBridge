//
// Created by uma on 2023/6/8.
//https://github.com/shank03/HMAC_SHA256
//

#ifndef AVBRIDGE_HMAC_SHA256_H
#define AVBRIDGE_HMAC_SHA256_H

#include <array>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>

// -------------------------------------------
// Author and Credits:
// https://github.com/System-Glitch/SHA256
// https://github.com/h5p9sl/hmac_sha256
// -------------------------------------------

class HMAC {
private:
    static const size_t SHA256_BLOCK_SIZE = 64;
    static const size_t SHA256_HASH_SIZE = 32;

    class SHA256 {
    public:
        SHA256();

        void update(const uint8_t *data, size_t length);

        uint8_t *digest();

    private:
        uint8_t m_data[64];
        uint32_t m_block_len;
        uint64_t m_bit_len;
        uint32_t m_state[8];    // A, B, C, D, E, F, G, H

        static constexpr std::array<uint32_t, 64> K = {
                0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        static uint32_t rot_r(uint32_t x, uint32_t n);

        static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);

        static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);

        static uint32_t sig0(uint32_t x);

        static uint32_t sig1(uint32_t x);

        void transform();

        void pad();

        void revert(uint8_t *hash);
    };

    static void H(const uint8_t *x, const uint8_t *y, size_t y_len, uint8_t *out);

    static void sha256(const uint8_t *data, size_t data_len, uint8_t *out, size_t out_len);

public:
    static uint8_t *hash(const uint8_t *key, size_t key_len, const uint8_t *data, size_t data_len);

    static std::string toString(const uint8_t *digest);
};
HMAC::SHA256::SHA256() : m_block_len(0),
                         m_bit_len(0) {
    m_state[0] = 0x6a09e667;
    m_state[1] = 0xbb67ae85;
    m_state[2] = 0x3c6ef372;
    m_state[3] = 0xa54ff53a;
    m_state[4] = 0x510e527f;
    m_state[5] = 0x9b05688c;
    m_state[6] = 0x1f83d9ab;
    m_state[7] = 0x5be0cd19;
}

void HMAC::SHA256::update(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        m_data[m_block_len++] = data[i];
        if (m_block_len == 64) {
            transform();

            // End of the block
            m_bit_len += 512;
            m_block_len = 0;
        }
    }
}

uint8_t *HMAC::SHA256::digest() {
    auto *hash = new uint8_t[32];
    pad();
    revert(hash);
    return hash;
}

uint32_t HMAC::SHA256::rot_r(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

uint32_t HMAC::SHA256::choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

uint32_t HMAC::SHA256::majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & (b | c)) | (b & c);
}

uint32_t HMAC::SHA256::sig0(uint32_t x) {
    return SHA256::rot_r(x, 7) ^ SHA256::rot_r(x, 18) ^ (x >> 3);
}

uint32_t HMAC::SHA256::sig1(uint32_t x) {
    return SHA256::rot_r(x, 17) ^ SHA256::rot_r(x, 19) ^ (x >> 10);
}

void HMAC::SHA256::transform() {
    uint32_t maj, xorA, ch, xorE, sum, newA, newE, m[64];
    uint32_t state[8];

    for (uint8_t i = 0, j = 0; i < 16; i++, j += 4) {    // Split data in 32 bit blocks for the 16 first words
        m[i] = (m_data[j] << 24) | (m_data[j + 1] << 16) | (m_data[j + 2] << 8) | (m_data[j + 3]);
    }

    for (uint8_t k = 16; k < 64; k++) {    // Remaining 48 blocks
        m[k] = SHA256::sig1(m[k - 2]) + m[k - 7] + SHA256::sig0(m[k - 15]) + m[k - 16];
    }

    for (uint8_t i = 0; i < 8; i++) {
        state[i] = m_state[i];
    }

    for (uint8_t i = 0; i < 64; i++) {
        maj  = SHA256::majority(state[0], state[1], state[2]);
        xorA = SHA256::rot_r(state[0], 2) ^ SHA256::rot_r(state[0], 13) ^ SHA256::rot_r(state[0], 22);

        ch = choose(state[4], state[5], state[6]);

        xorE = SHA256::rot_r(state[4], 6) ^ SHA256::rot_r(state[4], 11) ^ SHA256::rot_r(state[4], 25);

        sum  = m[i] + K[i] + state[7] + ch + xorE;
        newA = xorA + maj + sum;
        newE = state[3] + sum;

        state[7] = state[6];
        state[6] = state[5];
        state[5] = state[4];
        state[4] = newE;
        state[3] = state[2];
        state[2] = state[1];
        state[1] = state[0];
        state[0] = newA;
    }

    for (uint8_t i = 0; i < 8; i++) {
        m_state[i] += state[i];
    }
}

void HMAC::SHA256::pad() {
    uint64_t i   = m_block_len;
    uint8_t  end = m_block_len < 56 ? 56 : 64;

    m_data[i++] = 0x80;    // Append a bit 1
    while (i < end) {
        m_data[i++] = 0x00;    // Pad with zeros
    }

    if (m_block_len >= 56) {
        transform();
        memset(m_data, 0, 56);
    }

    // Append to the padding the total message's length in bits and transform.
    m_bit_len += m_block_len * 8;
    m_data[63] = m_bit_len;
    m_data[62] = m_bit_len >> 8;
    m_data[61] = m_bit_len >> 16;
    m_data[60] = m_bit_len >> 24;
    m_data[59] = m_bit_len >> 32;
    m_data[58] = m_bit_len >> 40;
    m_data[57] = m_bit_len >> 48;
    m_data[56] = m_bit_len >> 56;
    transform();
}

void HMAC::SHA256::revert(uint8_t *hash) {
    // SHA uses big endian byte ordering
    // Revert all bytes
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            hash[i + (j * 4)] = (m_state[j] >> (24 - i * 8)) & 0x000000ff;
        }
    }
}

std::string HMAC::toString(const uint8_t *digest) {
    std::stringstream s;
    s << std::setfill('0') << std::hex;

    for (uint8_t i = 0; i < 32; i++) {
        s << std::setw(2) << (unsigned int) digest[i];
    }

    return s.str();
}

uint8_t *HMAC::hash(const uint8_t *key, size_t key_len, const uint8_t *data, size_t data_len) {
    auto *out = new uint8_t[SHA256_HASH_SIZE];

    uint8_t k[SHA256_BLOCK_SIZE];
    uint8_t i_pad[SHA256_BLOCK_SIZE], o_pad[SHA256_BLOCK_SIZE];
    uint8_t i_hash[SHA256_HASH_SIZE], o_hash[SHA256_HASH_SIZE];

    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++) i_pad[i] = 0x36, o_pad[i] = 0x5c, k[i] = 0;

    if (key_len > SHA256_BLOCK_SIZE) {
        sha256(key, key_len, k, SHA256_BLOCK_SIZE);
    } else {
        memcpy(k, key, key_len);
    }

    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++) i_pad[i] ^= k[i], o_pad[i] ^= k[i];

    H(i_pad, data, data_len, i_hash);
    H(o_pad, i_hash, SHA256_HASH_SIZE, o_hash);

    memcpy(out, o_hash, SHA256_HASH_SIZE);
    return out;
}

void HMAC::H(const uint8_t *x, const uint8_t *y, size_t y_len, uint8_t *out) {
    size_t buff_len = SHA256_BLOCK_SIZE + y_len;
    auto  *buffer   = new uint8_t[buff_len];
    memcpy(buffer, x, SHA256_BLOCK_SIZE);
    memcpy(buffer + SHA256_BLOCK_SIZE, y, y_len);

    sha256(buffer, buff_len, out, SHA256_HASH_SIZE);
    delete[] buffer;
}

void HMAC::sha256(const uint8_t *data, size_t data_len, uint8_t *out, size_t out_len) {
    SHA256 sha;
    sha.update(data, data_len);
    out_len = (out_len > SHA256_HASH_SIZE) ? SHA256_HASH_SIZE : out_len;
    memcpy(out, sha.digest(), out_len);
}
#endif //AVBRIDGE_HMAC_SHA256_H