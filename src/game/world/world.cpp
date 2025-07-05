#include "world.hpp"
//
#include <iostream>
#include <TOML/cpptoml.h>
#include "game/generation/generation.hpp"

template <typename T>
concept AllowedType = std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t>;

///brief ID = uint8_t | uint16_t
template <AllowedType ID>
void loadSection(std::unordered_map<ID, std::string>& umap, const cpptoml::table& section) {
    for (const auto& [str_id, str_name] : section) {
        ID id = static_cast<ID>(std::stoul(str_id));
        std::string name = section.get_as<std::string>(str_id).value_or("");
        if (!name.empty())
            umap.try_emplace(id, name);
    }
}

static void load(ElementRegistry& reg) {
    const auto& root = cpptoml::parse_file("res/world_content.toml");
    loadSection(reg.floorTypes, *root->get_table("floor"));
    loadSection(reg.overlayTypes, *root->get_table("overlay"));
    loadSection(reg.blockTypes, *root->get_table("block"));
}

World::World(const TileCoord mapSize) : terrain(mapSize.x), mapSize(mapSize)  {
    load(registry);
    for (auto& line : terrain) {
        line.resize(mapSize.y);
    }
    gen::generate(terrain, mapSize);

    terrain[10][10].block = new Block("round_shadow", "ice_block", TileCoord(10, 10));
}

void World::print() {
    for (int y = 0; y < terrain.size(); ++y) {
        for (int x = 0; x < terrain[0].size(); ++x) {
            std::cout << static_cast<int>(terrain[x][y].floor);
        }
        std::cout << '\n';
    }
}

void World::placeFloor(const TileCoord tile, const uint8_t floorID) {
    if (tileExists(tile))
        terrain[tile.x][tile.y].floor = floorID;
}
void World::placeOverlay(const TileCoord tile, const uint8_t overlayID){
    if (tileExists(tile))
        terrain[tile.x][tile.y].overlay = overlayID;
}
