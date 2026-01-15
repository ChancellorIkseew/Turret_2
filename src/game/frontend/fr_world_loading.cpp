#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "layouts/l_saves.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

class FrWorldLoading : public Container{
    FrSaves* saves = nullptr;
public:
    FrWorldLoading(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        saves = addNode(new FrSaves());
        auto lower = addNode(new Layout(Orientation::horizontal));

        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, U"Load"))->addCallback([&] { saves->loadWorld(engine); });
        lower->addNode(new Button(BTN_SIZE, U"Delete"))->addCallback([&] { deleteWorld(); });

        arrange();
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
