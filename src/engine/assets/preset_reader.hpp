#pragma once
#include <span>
#include "engine/debug/logger.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/render/atlas.hpp"
#include "game/physics/physics_base.hpp"

class PresetReader {
    const tin::Data& data;
    const std::string& fileName;
    bool& failed;
    mutable debug::Logger logger;
public:
    PresetReader(const tin::Data& data, const std::string& fileName, bool& failed)
        : data(data), fileName(fileName), failed(failed), logger("preset_reader") { }

    template<typename T>
    T get(std::optional<T>(tin::Data::* func)(const std::string&) const, const std::string& key) const {
        auto res = (data.*func)(key);
        if (!res) {
            logger.error() << "[" << fileName << "] Missing or invalid key: " << key;
            failed = true;
            throw std::bad_optional_access();
        }
        return *res;
    }

    std::string getString(const std::string& key) const {
        auto res = data.getString(key);
        if (!res) {
            logger.error() << "[" << fileName << "] Missing key: " << key;
            failed = true;
            throw std::bad_optional_access();
        }
        return *res;
    }

    Texture getTexture(const Atlas& atlas, const std::string& key) const {
        std::string textureName = getString(key);
        Texture texture = atlas.at(textureName);
        if (texture == NULL_TEXTURE) {
            logger.error() << "[" << fileName << "] Texture not found in atlas: " << textureName;
            failed = true;
            throw std::bad_optional_access();
        }
        return texture;
    }

    PresetID getID(const std::unordered_map<std::string, PresetID>& idMap, const std::string& key) const {
        std::string targetName = getString(key);
        if (!idMap.contains(targetName)) {
            logger.error() << "[" << fileName << "] Dependency not found: " << targetName;
            failed = true;
            throw std::bad_optional_access();
        }
        return idMap.at(targetName);
    }

    size_t getPixelCoordArray(const std::string& key, std::span<PixelCoord> outArray) const {
        std::vector<std::string> list = data.getList(key);

        if (list.empty()) {
            logger.error() << "[" << fileName << "] List is empty or missing: " << key;
            failed = true;
            throw std::bad_optional_access();
        }

        if (list.size() > outArray.size()) {
            logger.error() << "[" << fileName << "] Too many elements in [" << key
                << "]. Found: " << list.size() << ", Max: " << outArray.size();
            failed = true;
            throw std::bad_optional_access();
        }

        for (size_t i = 0; i < list.size(); ++i) {
            auto coord = validator::toPixelCoord(list[i]);
            if (!coord) {
                logger.error() << "[" << fileName << "] Invalid PixelCoord format in list ["
                    << key << "] at index " << i << ": '" << list[i] << "'";
                failed = true;
                throw std::bad_optional_access();
            }
            outArray[i] = *coord;
        }

        return list.size();
    }

    size_t getUint8Array(const std::string& key, std::span<uint8_t> outArray) const {
        std::vector<std::string> list = data.getList(key);

        if (list.empty()) {
            logger.error() << "[" << fileName << "] List is empty or missing: " << key;
            failed = true;
            throw std::bad_optional_access();
        }

        if (list.size() > outArray.size()) {
            logger.error() << "[" << fileName << "] Too many elements in [" << key
                << "]. Found: " << list.size() << ", Max: " << outArray.size();
            failed = true;
            throw std::bad_optional_access();
        }

        for (size_t i = 0; i < list.size(); ++i) {
            auto u = validator::toUint8(list[i]);
            if (!u) {
                logger.error() << "[" << fileName << "] Invalid PixelCoord format in list ["
                    << key << "] at index " << i << ": '" << list[i] << "'";
                failed = true;
                throw std::bad_optional_access();
            }
            outArray[i] = *u;
        }

        return list.size();
    }
};
