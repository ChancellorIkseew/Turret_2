#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "validator.hpp"
#include "engine/coords/pixel_coord.hpp"
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
        void emplace(std::string key, const char* value) {
            data.emplace(std::move(key), value);
        }
        template<class T>
        void emplace(const std::string key, const T value) {
            data.emplace(std::move(key), std::to_string(value));
        }
        template<class T, class U>
        void get(const std::string& key, T& outValue, const U alternative) const {
            checkTypesAssert<T, U>();
            if (!data.contains(key))
                outValue = static_cast<T>(alternative);
            else
                outValue = validator::to<T>(data.at(key)).value_or(static_cast<T>(alternative));
        }

        template<class T>
        std::optional<T> get(const std::string& key) const {
            if (!data.contains(key)) return std::nullopt;
            return validator::to<T>(data.at(key));
        }

        std::vector<std::string> getList(const std::string& key) const {
            std::vector<std::string> list;
            if (!data.contains(key)) return list;
            //
            std::stringstream ss(data.at(key));
            std::string line;
            while (std::getline(ss, line, ',')) {
                list.push_back(line);
            }
            return list;
        }

        auto cbegin() const noexcept { return data.cbegin(); }
        auto begin()  const noexcept { return data.begin(); }
        auto begin()  noexcept { return data.begin(); }
        auto cend()   const noexcept { return data.cend(); }
        auto end()    const noexcept { return data.end(); }
        auto end()    noexcept { return data.end(); }
    private:
        template<class T, class U>
        constexpr void checkTypesAssert() const {
            static_assert(std::is_convertible_v<U, T>,
                "Alternative must be convertible to out value");
            static_assert(std::is_same_v<T, bool> == std::is_same_v<U, bool>,
                "If out value is bool, alternative olso must be bool");
            if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
                static_assert(std::is_unsigned_v<T> == std::is_unsigned_v<U>,
                    "Out value and alternative must be both signed or unsigned");
            }
        }
    };

    enum class Log : uint8_t { only_error, error_and_success };
    void write(std::filesystem::path path, const Data& data, const Log log = Log::error_and_success);
    Data read(std::filesystem::path path, const Log log = Log::error_and_success);
}
