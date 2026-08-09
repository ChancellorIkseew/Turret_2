#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/form.hpp"
#include "layouts/l_saves.hpp"

constexpr Point BTN_SIZE(120, 30);

class FrWorldSaving : public Container {
    FrSaves* saves = nullptr;
    Form* worldName = nullptr;
public:
    FrWorldSaving(Engine& engine) : Container(Align::center, Orientation::vertical) {
        saves = addNode(new FrSaves());
        auto lower = addNode(new Layout(Orientation::horizontal));

        lower->addNode(new Button(BTN_SIZE, tr("Back")))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, tr("Save")))->addCallback([&] { saveWorld(engine); });
        worldName = lower->addNode(new Form());
    }
private:
    void saveWorld(Engine& engine) {
        saves->saveWorld(engine, worldName->getText());
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initWorldSaving(Engine& engine) {
    return std::make_unique<FrWorldSaving>(engine);
}
