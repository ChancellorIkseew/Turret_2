#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include "validator.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/io/utf8/utf8.hpp"

namespace tin {
    class Data {
        std::unordered_map<std::string, std::string> data;
    public:
        Data() = default;
        bool empty() { return data.empty(); }
        void emplace(std::string key, std::string value) {
            data.emplace(std::move(key), std::move(value));
        }

        std::optional<uint64_t> getUint64(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint64(data.at(key));
        }
        std::optional<uint32_t> getUint32(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint32(data.at(key));
        }
        std::optional<uint16_t> getUint16(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint16(data.at(key));
        }
        std::optional<uint8_t> getUint8(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint8(data.at(key));
        }

        std::optional<int64_t> getInt64(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt64(data.at(key));
        }
        std::optional<int32_t> getInt32(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt32(data.at(key));
        }
        std::optional<int16_t> getInt16(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt16(data.at(key));
        }
        std::optional<int8_t> getInt8(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt8(data.at(key));
        }

        std::optional<float> getFloat(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toFloat(data.at(key));
        }
        std::optional<bool> getBool(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toBool(data.at(key));
        }
        std::optional<std::string> getString(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return data.at(key);
        }
        std::optional<std::u32string> getU32String(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return utf8::to_u32string(data.at(key));
        }
        std::optional<TileCoord> getTileCoord(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toTileCoord(data.at(key));
        }

        auto cbegin() const noexcept { return data.cbegin(); }
        auto begin()  const noexcept { return data.begin(); }
        auto begin()  noexcept { return data.begin(); }
        auto cend()   const noexcept { return data.cend(); }
        auto end()    const noexcept { return data.end(); }
        auto end()    noexcept { return data.end(); }
    };

    void write(std::filesystem::path path, const Data& data);
    Data read(std::filesystem::path path);
}
