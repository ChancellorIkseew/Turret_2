#pragma once
#include <memory>
#include <unordered_map>
#include "team.hpp"

class TeamsPool {
    std::unordered_map<TeamID, std::unique_ptr<Team>> teams;
    TeamID nextID = 0;
public:
    TeamsPool() = default;
    ///@brief Game can not create more, then 256 teams in one session.
    Team* addTeam(const std::u32string& name) {
        teams.try_emplace(nextID, std::make_unique<Team>(name, nextID));
        return teams.at(nextID++).get();
    }
    Team* getTeamByID(const TeamID ID) { return teams.at(ID).get(); }
    bool containsID(const TeamID ID) const { return teams.contains(ID); }
    //
    auto cbegin() const noexcept { return teams.cbegin(); }
    auto begin()  const noexcept { return teams.begin(); }
    auto begin()  noexcept { return teams.begin(); }
    auto cend()   const noexcept { return teams.cend(); }
    auto end()    const noexcept { return teams.end(); }
    auto end()    noexcept { return teams.end(); }
private:
    TeamsPool(const TeamsPool&) = delete;
    TeamsPool(TeamsPool&&) = delete;
    TeamsPool& operator=(const TeamsPool&) = delete;
    TeamsPool& operator=(TeamsPool&&) = delete;
};
