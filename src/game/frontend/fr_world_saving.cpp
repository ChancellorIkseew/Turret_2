#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/form.hpp>
#include "layouts/l_saves.hpp"

constexpr Point BTN_SIZE(120, 30);

class FrWorldSaving : public Container {
    FrSaves* saves = nullptr;
    Form* worldName = nullptr;
public:
    FrWorldSaving(App& app) : Container(Align::center, Orientation::vertical) {
        saves = addNode(new FrSaves());
        auto lower = addNode(new Layout(Orientation::horizontal));

        lower->addNode(new Button(BTN_SIZE, tr("Back")))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, tr("Save")))->addCallback([&] { saveWorld(app); });
        worldName = lower->addNode(new Form());
    }
private:
    void saveWorld(App& app) {
        saves->saveWorld(app, worldName->getText());
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initWorldSaving(App& app) {
    return std::make_unique<FrWorldSaving>(app);
}
