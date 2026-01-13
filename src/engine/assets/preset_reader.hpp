#pragma once
#include <span>
#include "engine/debug/logger.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/render/atlas.hpp"
#include "game/physics/physics_base.hpp"
#include "preset_id.hpp"

template<class Tag>
using FindMap = std::unordered_map<std::string, preset_tag::StrongID<Tag>>;

class PresetReader {
    const tin::Data& data;
    const std::string& fileName;
    bool& failed;
    mutable debug::Logger logger;
public:
    PresetReader(const tin::Data& data, const std::string& fileName, bool& failed)
        : data(data), fileName(fileName), failed(failed), logger("preset_reader") { }

    template<typename T>
    T get(const std::string& key) const {
    std::optional<T> res = data.get<T>(key);
        if (!res)
            fail("Missing or invalid key: " + key);
        return *res;
    }

    Texture getTexture(const Atlas& atlas, const std::string& key) const {
        std::string textureName = get<std::string>(key);
        Texture texture = atlas.at(textureName);
        if (texture == NULL_TEXTURE)
            fail("Texture not found in atlas: " + textureName);
        return texture;
    }

    template<class Tag>
    preset_tag::StrongID<Tag> getID(const FindMap<Tag>& idMap, const std::string& key) const {
        std::string targetName = get<std::string>(key);
        if (!idMap.contains(targetName))
            fail( "Dependency not found: " + targetName);
        return idMap.at(targetName);
    }

    template<typename T>
    size_t getArray(const std::string& key, std::span<T> outArray) const {
        std::vector<std::string> list = data.getList(key);

        if (list.empty())
            fail("List is empty or missing: " + key);

        if (list.size() > outArray.size())
            fail("Too many elements in [" + key + "]. Found: " +
                std::to_string(list.size()) + ", Max: " + std::to_string(outArray.size()));

        for (size_t i = 0; i < list.size(); ++i) {
            auto val = validator::to<T>(list[i]);
            if (!val)
                fail("Invalid format in list [" + key + "] at index " +
                    std::to_string(i) + ": '" + list[i] + "'");
            outArray[i] = *val;
        }
        return list.size();
    }
private:
    [[noreturn]] void fail(const std::string& message) const {
        logger.error() << "[" << fileName << "] " << message;
        failed = true;
        throw std::bad_optional_access();
    }
};
