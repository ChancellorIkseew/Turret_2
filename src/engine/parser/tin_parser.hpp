#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include "validator.hpp"
#include "engine/coords/tile_coord.hpp"

namespace tin {
    class Data {
        std::unordered_map<std::string, std::string> data;
    public:
        Data() = default;
        void emplace(std::string key, std::string value) {
            data.emplace(std::move(key), std::move(value));
        }

        std::optional<uint64_t> getUint64(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint64(data.at(key));
        }
        std::optional<uint32_t> getUint32(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint32(data.at(key));
        }
        std::optional<uint16_t> getUint16(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint16(data.at(key));
        }
        std::optional<uint8_t> getUint8(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toUint8(data.at(key));
        }

        std::optional<int64_t> getInt64(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt64(data.at(key));
        }
        std::optional<int32_t> getInt32(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt32(data.at(key));
        }
        std::optional<int16_t> getInt16(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt16(data.at(key));
        }
        std::optional<int8_t> getInt8(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toInt8(data.at(key));
        }

        std::optional<float> getFloat(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toFloat(data.at(key));
        }
        std::optional<bool> getBool(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::toBool(data.at(key));
        }
        std::optional<std::string> getString(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            return data.at(key);
        }
        std::optional<TileCoord> getTileCoord(const char* key) const {
            if (!data.contains(key)) return std::nullopt;
            const std::string& value = data.at(key);
            const size_t stick = value.find_first_of('|');
            if (stick == value.npos)
                return std::nullopt;
            const auto x = validator::toInt32(value.substr(0, stick));
            const auto y = validator::toInt32(value.substr(stick + 1));
            if (!x.has_value() || !y.has_value())
                return std::nullopt;
            return TileCoord(x.value(), y.value());
        }

        auto cbegin() const noexcept { return data.cbegin(); }
        auto begin()  const noexcept { return data.begin(); }
        auto begin()  noexcept { return data.begin(); }
        auto cend()   const noexcept { return data.cend(); }
        auto end()    const noexcept { return data.end(); }
        auto end()    noexcept { return data.end(); }
    };

    void write(std::filesystem::path path, const Data& data);
    std::optional<Data> read(std::filesystem::path path);
};
