#include "SHA256.h"


void SHA256::init() {
    state[0] = 0x6a09e667;
    state[1] = 0xbb67ae85;
    state[2] = 0x3c6ef372;
    state[3] = 0xa54ff53a;
    state[4] = 0x510e527f;
    state[5] = 0x9b05688c;
    state[6] = 0x1f83d9ab;
    state[7] = 0x5be0cd19;
    dataLen = 0;
    bitLen = 0;
}
SHA256::SHA256() { init(); }

void SHA256::update(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        dataBuffer[dataLen++] = data[i];
        if (dataLen == 64) {
            transform();
            bitLen += 512;
            dataLen = 0;
        }
    }
}

void SHA256::update(const std::string& data) {
    update(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());
}

std::string SHA256::final() {
    uint8_t hash[32];
    pad();
    transform();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            hash[i + j * 4] = (state[j] >> (24 - i * 8)) & 0x000000ff;
        }
    }
    return bytesToHex(hash, 32);
}
    void SHA256::transform() {
        uint32_t m[64];
        for (int i = 0; i < 16; ++i) {
            m[i] = (dataBuffer[i * 4] << 24) | (dataBuffer[i * 4 + 1] << 16) |
                (dataBuffer[i * 4 + 2] << 8) | (dataBuffer[i * 4 + 3]);
        }
        for (int i = 16; i < 64; ++i) {
            m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
        }

        uint32_t a, b, c, d, e, f, g, h;
        a = state[0];
        b = state[1];
        c = state[2];
        d = state[3];
        e = state[4];
        f = state[5];
        g = state[6];
        h = state[7];

        for (int i = 0; i < 64; ++i) {
            uint32_t temp1 = h + EP1(e) + CH(e, f, g) + K[i] + m[i];
            uint32_t temp2 = EP0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
        state[5] += f;
        state[6] += g;
        state[7] += h;
    }

    void SHA256::pad() {
        uint64_t i = dataLen;
        uint8_t end = dataLen < 56 ? 56 : 64;
        dataBuffer[i++] = 0x80;
        while (i < end) dataBuffer[i++] = 0x00;
        if (dataLen >= 56) {
            transform();
            memset(dataBuffer, 0, 56);
        }
        bitLen += dataLen * 8;
        dataBuffer[63] = bitLen;
        dataBuffer[62] = bitLen >> 8;
        dataBuffer[61] = bitLen >> 16;
        dataBuffer[60] = bitLen >> 24;
        dataBuffer[59] = bitLen >> 32;
        dataBuffer[58] = bitLen >> 40;
        dataBuffer[57] = bitLen >> 48;
        dataBuffer[56] = bitLen >> 56;
    }

    std::string SHA256::bytesToHex(const uint8_t* bytes, size_t length) {
        std::ostringstream oss;
        for (size_t i = 0; i < length; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
        }
        return oss.str();
    }