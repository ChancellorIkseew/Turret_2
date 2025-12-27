#pragma once
#include <limits>
#include <type_traits>

template<class IDCandidate>
concept CanBeID = std::is_integral_v<IDCandidate>;

template<CanBeID ID>
class IDManager {
    std::vector<ID> recycledIDs;
    ID nextID = 0;
public:
    static constexpr ID INVALID_ID = std::numeric_limits<ID>::max();
    static constexpr ID MAX_ID = INVALID_ID - 1;
public:
    IDManager() = default;
    IDManager(const size_t recycledIDsCapacity) {
        recycledIDs.reserve(recycledIDsCapacity);
    }
    //
    ID getNext() noexcept {
        if (!recycledIDs.empty()) {
            ID id = recycledIDs.back();
            recycledIDs.pop_back();
            return id;
        }
        if (nextID <= MAX_ID)
            return nextID++;
        return INVALID_ID;
    }
    void setFree(const ID id) {
        recycledIDs.push_back(id);
    }
};
