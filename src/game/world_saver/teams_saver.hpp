#pragma once
#include "engine/io/io.hpp"
#include "engine/io/parser/jtin_parser.hpp"
#include "game/common/teams_pool.hpp"

inline void saveTeams(const TeamsPool& pool, const std::filesystem::path& path) {
    jtin::Data data;
    for (const auto& [id, team] : pool) {
        tin::Data section;
        section.emplace("id", id);
        auto& resources = team->getInventory().resources;
        for (size_t i = 0; i < resources.size(); ++i) {
            if (resources[i] > 0)
                section.emplace(std::to_string(i), resources[i]);
        }
        data.push_back(section);
    }
    jtin::write(path / "teams.jtin", data);
}

inline void loadTeams(TeamsPool& pool, const std::filesystem::path& path) {
    jtin::Data data = jtin::read(path / "teams.jtin");
    for (const auto& section : data) {
        TeamID id = section.get<TeamID>("id").value();
        pool.addTeam(id);
        Team* team = pool.getTeamByID(id);
        auto& resources = team->getInventory().resources;
        for (size_t i = 0; i < resources.size(); ++i) {
            resources[i] = section.get<int64_t>(std::to_string(i)).value_or(0);
        }
    }
}
