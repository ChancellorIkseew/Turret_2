#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include "app.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
#include "layouts/l_saves.hpp"

constexpr Point BTN_SIZE(120, 30);

class FrWorldLoading : public Container {
    FrSaves* saves = nullptr;
public:
    FrWorldLoading(App& app) : Container(Align::center, Orientation::vertical) {
        saves = addNode(new FrSaves());
        auto lower = addNode(new Layout(Orientation::horizontal));

        lower->addNode(new Button(BTN_SIZE, tr("Back")))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, tr("Load")))->addCallback([&] { loadWorld(app); });
        lower->addNode(new Button(BTN_SIZE, tr("Delete")))->addCallback([&] { deleteWorld(app); });
    }

    void callback(UIContext& context) final {
        Container::callback(context);
    }
private:
    void loadWorld(App& app) {
        if (saves->getTargetFolder().empty()) {
            app.getGUI().addToOverlay(frontend::initMessageBox(app, tr("Select a world to load")));
            return;
        }
        app.loadWorldInGame(saves->getTargetFolder());
    }

    void deleteWorldFolder(App& app, std::string /*not ref*/ folder) {
        io::folders::deleteFolder(io::folders::SAVES / folder);
        app.getGUI().addToOverlay(frontend::initMessageBox(app, tr("World deleted")));
        saves->refresh();
        markDirty();
    }

    void deleteWorld(App& app) {
        const std::string folder = saves->getTargetFolder();
        if (folder.empty()) {
            app.getGUI().addToOverlay(frontend::initMessageBox(app, tr("Select a world to delete.")));
            return;
        }
        app.getGUI().addToOverlay(frontend::initConfirmDialog(app, tr("Delete world '{}'?", folder),
            [&, folder] { deleteWorldFolder(app, folder);}));
    }
};

std::unique_ptr<Container> frontend::initWorldLoading(App& app) {
    return std::make_unique<FrWorldLoading>(app);
}
