#include "engine/widgets/selector.hpp"

namespace frontend {
    void update(Selector* saves, std::string& folder);
    //
    std::unique_ptr<Selector> initSaves(std::string& folder);
}
