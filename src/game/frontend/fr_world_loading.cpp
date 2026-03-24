#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "layouts/l_saves.hpp"

constexpr Point BTN_SIZE(120.0f, 30.0f);

class FrWorldLoading : public Container{
    FrSaves* saves = nullptr;
public:
    FrWorldLoading(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        saves = addNode(new FrSaves());
        auto lower = addNode(new Layout(Orientation::horizontal));

        lower->addNode(new Button(BTN_SIZE, "Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, "Load"))->addCallback([&] { saves->loadWorld(engine); });
        lower->addNode(new Button(BTN_SIZE, "Delete"))->addCallback([&] { deleteWorld(); });
    }
private:
    void deleteWorld() {
        saves->deleteWorld();
        arrange();
    }
};

std::unique_ptr<Container> frontend::initWorldLoading(Engine& engine) {
    return std::make_unique<FrWorldLoading>(engine);
}
