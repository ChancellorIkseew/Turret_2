#include "utf8.hpp"

uint32_t utf8::fromConstCharToUint32(const char* s) noexcept {
    if (!s || *s == '\0') return 0;

    const uint8_t u0 = static_cast<uint8_t>(s[0]);
    if (u0 < 0x80) return u0;  // 1-byte ASCII.

    size_t len = 0;
    uint32_t cp = 0;

    if      (u0 < 0xE0) { len = 2; cp = u0 & 0x1F; }
    else if (u0 < 0xF0) { len = 3; cp = u0 & 0x0F; }
    else if (u0 < 0xF8) { len = 4; cp = u0 & 0x07; }
    else return 0;  // Invalid starter byte.

    for (size_t i = 1; i < len; ++i) {
        if (!s[i]) return 0;  // Unexpected end
        uint8_t ui = static_cast<uint8_t>(s[i]);
        if ((ui & 0xC0) != 0x80) return 0;  // Bad continuation.
        cp = (cp << 6) | (ui & 0x3F);
    }

    // Validate ranges.
    if ((len == 2 && cp < 0x80) ||   // Overlong.
        (len == 3 && cp < 0x800) ||
        (len == 4 && (cp < 0x10000 || cp > 0x10FFFF)) ||
        (cp >= 0xD800 && cp <= 0xDFFF)) {
        return 0;
    }

    return cp;
}

std::u32string utf8::fromConstCharToU32String(const char* cStr) {
    std::u32string result;
    while (*cStr != '\0') {
        result.push_back(static_cast<unsigned char>(*cStr));
        ++cStr;
    }
    return result;
}
