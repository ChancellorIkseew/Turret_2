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

std::u32string utf8::to_u32string(const char* cStr) {
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

static void appendUTF8Char(std::string& result, char32_t cp) {
    if (cp < 0x00000000 || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF))
        return;

    if (cp <= 0x7F) // 1-byte ASCII.
        result.push_back(static_cast<char>(cp));
    else if (cp <= 0x7FF) { // 2-byte UTF-8.
        result.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
    else if (cp <= 0xFFFF) { // 3-byte UTF-8.
        result.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
    else { // 4-byte UTF-8. (cp <= 0x10FFFF)
        result.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        result.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
}

std::string utf8::to_string(const std::u32string& u32Str) {
    std::string result;
    if (u32Str.empty()) return result;
    for (char32_t codePoint : u32Str) {
        appendUTF8Char(result, codePoint);
    }
    return result;
}
