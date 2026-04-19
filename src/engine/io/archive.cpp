#include "archive.hpp"
//
#include <ZLIB/zlib.h>
#include <cstdint>

std::string archive::pack(std::string_view source) {
    const uLongf srcLen = static_cast<uLong>(source.size());
    uLongf destLen = compressBound(srcLen);
    const uint64_t originalSize = srcLen;
    //
    std::string buffer;
    buffer.resize(sizeof(uint64_t) + destLen);
    std::memcpy(buffer.data(), &originalSize, sizeof(uint64_t));
    if (srcLen == 0)
        destLen = 0;
    else {
        Bytef* dst = reinterpret_cast<Bytef*>(buffer.data() + sizeof(uint64_t));
        const Bytef* src = reinterpret_cast<const Bytef*>(source.data());
        int result = compress(dst, &destLen, src, srcLen);
        if (result != Z_OK) throw std::runtime_error("Failed to pack");
    }
    //
    buffer.resize(sizeof(uint64_t) + destLen);
    return buffer;
}

std::string archive::unpack(std::string_view packed) {
    if (packed.size() < sizeof(uint64_t)) throw std::runtime_error("Corrupted data");
    //    
    uint64_t originalSize = 0;
    std::memcpy(&originalSize, packed.data(), sizeof(uint64_t));
    packed.remove_prefix(sizeof(uint64_t));
    //
    std::string buffer;
    buffer.resize(originalSize);
    uLongf dstSize = static_cast<uLongf>(originalSize);
    Bytef* dst = reinterpret_cast<Bytef*>(buffer.data());
    const Bytef* src = reinterpret_cast<const Bytef*>(packed.data());
    int result = uncompress(dst, &dstSize, src, static_cast<uLong>(packed.size()));
    //
    if (result != Z_OK) throw std::runtime_error("Failed to unpack");
    return buffer;
}
