#pragma once
#include <span>
#include "engine/debug/logger.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/render/atlas.hpp"
#include "game/common/physics_base.hpp"
#include "preset_id.hpp"

template<class Tag>
using FindMap = std::unordered_map<std::string, preset_tag::StrongID<Tag>>;

class PresetReader {
    const tin::Data& data;
    const std::string& fileName;
    mutable debug::Logger logger;
public:
    PresetReader(const tin::Data& data, const std::string& fileName)
        : data(data), fileName(fileName), logger("preset_reader") { }

    template<typename T>
    T getOpt(const std::string& key) const {
        std::optional<T> res = data.get<T>(key);
        if (!res)
            return T();
        return *res;
    }

    template<typename T>
    T get(const std::string& key) const {
        std::optional<T> res = data.get<T>(key);
        if (!res)
            fail("Missing or invalid key: " + key);
        return *res;
    }

    TextureRect getTexture(const Atlas& atlas, const std::string& key) const {
        std::string textureName = get<std::string>(key);
        TextureRect textureRect = atlas.at(textureName);
        if (textureRect == NULL_TEXTURE_RECT)
            fail("Texture not found in atlas: " + textureName);
        return textureRect;
    }

    template<class Tag>
    preset_tag::StrongID<Tag> getID(const FindMap<Tag>& idMap, const std::string& key) const {
        std::string targetName = get<std::string>(key);
        if (!idMap.contains(targetName))
            fail( "Dependency not found: " + targetName);
        return idMap.at(targetName);
    }

    template<typename T>
    uint8_t getOptArray(const std::string& key, std::span<T> outArray) const {
        std::vector<std::string> list = data.getList(key);

        if (list.empty())
            return 0;

        if (list.size() > outArray.size())
            fail(std::format("Too many elements in [{}]. Found: {} Max: {}", key, list.size(), outArray.size()));

        for (size_t i = 0; i < list.size(); ++i) {
            auto val = validator::to<T>(list[i]);
            if (!val)
                fail(std::format("Invalid format in list [{}] at index {}: '{}'", key, i, list[i]));
            outArray[i] = *val;
        }
        return static_cast<uint8_t>(list.size());
    }

    template<typename T>
    uint8_t getArray(const std::string& key, std::span<T> outArray) const {
        const uint8_t arraySize = getOptArray(key, outArray);
        if (arraySize < 1)
            fail("List is empty or missing: " + key);
        return arraySize;
    }
private:
    [[noreturn]] void fail(const std::string& message) const {
        logger.error("[{}] {}", fileName, message);
        throw std::bad_optional_access();
    }
};
