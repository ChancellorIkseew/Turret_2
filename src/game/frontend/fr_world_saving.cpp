#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "layouts/l_saves.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

class FrWorldSaving : public Container {
    FrSaves* saves = nullptr;
    Form* worldName = nullptr;
public:
    FrWorldSaving(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        saves = addNode(new FrSaves());
        auto lower = addNode(new Layout(Orientation::horizontal));

        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, U"Save"))->addCallback([&] { saveWorld(engine); });
        worldName = lower->addNode(new Form());

        arrange();
    }
private:
    void saveWorld(Engine& engine) {
        saves->saveWorld(engine, utf8::to_string(worldName->getText()));
        arrange();
    }
};

std::unique_ptr<Container> frontend::initWorldSaving(Engine& engine) {
    return std::make_unique<FrWorldSaving>(engine);
}
