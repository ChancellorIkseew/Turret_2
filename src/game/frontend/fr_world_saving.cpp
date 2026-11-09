#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/form.hpp>
#include "app.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
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
        saves->onSelect = [&](const std::string& name) { worldName->setText(name); };
    }
private:
    void saveWorld(App& app) {
        const std::string folder = worldName->getText();
        if (folder.empty()) {
            app.getGUI().addToOverlay(frontend::initMessageBox(
                tr("Enter a world name for a new save or choose existing world to rewrite.")));
            return;
        }
        const bool success = app.saveWorld(folder);
        saves->refresh();
        const std::string message = success
            ? tr("World saved to {}", std::filesystem::absolute(io::folders::SAVES / folder).string())
            : tr("Failed to save world.");
        app.getGUI().addToOverlay(frontend::initMessageBox(message));
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initWorldSaving(App& app) {
    return std::make_unique<FrWorldSaving>(app);
}
