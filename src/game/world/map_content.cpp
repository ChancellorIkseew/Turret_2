#include "map_content.hpp"
//
#include <TOML/cpptoml.h>

template <typename T>
concept AllowedType = std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t>;

///brief ID = uint8_t | uint16_t
template <AllowedType ID>
static void loadSection(std::unordered_map<ID, std::string>& umap, const cpptoml::table& section) {
    for (const auto& [str_id, str_name] : section) {
        ID id = static_cast<ID>(std::stoul(str_id));
        std::string name = section.get_as<std::string>(str_id).value_or("");
        if (!name.empty())
            umap.try_emplace(id, name);
    }
}

void content::load(ElementRegistry& reg) {
    const auto& root = cpptoml::parse_file("res/world_content.toml");
    loadSection(reg.floorTypes, *root->get_table("floor"));
    loadSection(reg.overlayTypes, *root->get_table("overlay"));
    loadSection(reg.blockTypes, *root->get_table("block"));
}
