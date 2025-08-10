#include "engine/widgets/layout.hpp"

namespace frontend {
    void update(Layout* saves, std::string& folder);
    //
    std::unique_ptr<Layout> initSaves(std::string& folder);
}
