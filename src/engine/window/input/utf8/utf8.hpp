#pragma once
#include <string>
#include <typeinfo>

namespace utf8 {
    uint32_t fromConstCharToUint32(const char* s) noexcept;
    ///@brief Only for one-byte-symbols(basic latin) strings.
    std::u32string fromConstCharToU32String(const char* cStr);
}
