#pragma once
#include "team.hpp"

class TeamsPool {
    std::unordered_map<TeamID, Team> teams;
    TeamID nextID = 0;
public:
    ///@brief Game can not create more, then 256 teams in one session.
    void addTeam(const std::u32string& name) {
        teams.try_emplace(nextID, Team(name, nextID));
        ++nextID;
    }
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
