#include "utf8.hpp"

char32_t utf8::to_char32_t(const char* s) noexcept {
    if (!s || *s == '\0') return 0;

    const uint8_t u0 = static_cast<uint8_t>(s[0]);
    if (u0 < 0x80) return u0; // 1-byte ASCII.

    size_t len = 0;
    char32_t cp = 0;

    if      (u0 < 0xE0) { len = 2; cp = u0 & 0x1F; }
    else if (u0 < 0xF0) { len = 3; cp = u0 & 0x0F; }
    else if (u0 < 0xF8) { len = 4; cp = u0 & 0x07; }
    else return 0; // Invalid starter byte.

    for (size_t i = 1; i < len; ++i) {
        if (!s[i]) return 0; // Unexpected end.
        uint8_t ui = static_cast<uint8_t>(s[i]);
        if ((ui & 0xC0) != 0x80) return 0; // Bad continuation.
        cp = (cp << 6) | (ui & 0x3F);
    }

    // Validate ranges.
    if ((len == 2 && cp < 0x80) || // Overlong.
        (len == 3 && cp < 0x800) ||
        (len == 4 && (cp < 0x10000 || cp > 0x10FFFF)) ||
        (cp >= 0xD800 && cp <= 0xDFFF)) {
        return 0;
    }

    return cp;
}

std::u32string utf8::to_u32String(const char* cStr) {
    std::u32string result;
    if (!cStr) return result;

    const char* current = cStr;
    while (*current != '\0') {
        char32_t codePoint = to_char32_t(current);
        if (codePoint == 0) {
            current += 1;
            continue;
        }

        result.push_back(codePoint);
        const uint8_t u0 = static_cast<uint8_t>(*current);
        if (u0 < 0x80)      current += 1; // 1-byte ASCII.
        else if (u0 < 0xE0) current += 2; // 2-byte UTF-8.
        else if (u0 < 0xF0) current += 3; // 3-byte UTF-8.
        else if (u0 < 0xF8) current += 4; // 4-byte UTF-8.
        else current += 1;
    }
    return result;
}
