#include "gui.hpp"
//
#include "engine/debug/console.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"
#include "engine/settings/settings.hpp"
#include "engine/util/time.hpp"
#include "engine/window/input/input.hpp"
#include "engine/window/window.hpp"

constexpr uint32_t BLACK = 0x00'00'00'FF;
constexpr PixelCoord DEBUD_PANEL_SIZE(200.f, 100.f);
static tin::Data langTranslations;

static void drawDebugPanel(const Renderer& renderer, const MainWindow& mainWindow) {
    PixelCoord position = PixelCoord(mainWindow.getSize().x - DEBUD_PANEL_SIZE.x, 0.f);
    renderer.drawRect(BLACK, position, DEBUD_PANEL_SIZE);
    position += PixelCoord(20.f, 20.f);
    text::drawString(renderer, U"FPS|TPS: " + utf8::to_u32string(1000U / mainWindow.getRealFrameDelay()), position);
    position.y += 20.f;
    text::drawString(renderer, U"Frame|tick time: " + utf8::to_u32string(mainWindow.getRealFrameDelay()), position);
}

void GUI::draw(const Renderer& renderer, const Atlas& atlas) {
    if (mainWindow.justResized())
        relocateContainers();
    //
    if (showGUI) {
        for (const auto& it : containers) {
            it->draw(renderer);
        }
        if (!overlaped.empty())
            overlaped.back()->draw(renderer);
    }
    if (showFPS)
        drawDebugPanel(renderer, mainWindow);
    if (showAtlas)
        renderer.drawFast(atlas.getComonTexture(), PixelCoord(0, 0), atlas.getSize());  
}

void GUI::translate() {
    for (auto& it : containers) {
        it->translate(langTranslations);
    }
    for (auto& it : overlaped) {
        it->translate(langTranslations);
    }
    relocateContainers();
}

void GUI::loadLangTranslations(const std::string& lang) {
    langTranslations = tin::read(io::folders::LANG / (lang + ".tin"));
}

void GUI::callback() {
    acceptHotkeys();
    if (!showGUI)
        return;
    UIContext context(engine.getAssets().getAudio(), mainWindow.getCursor(), input);
    if (!overlaped.empty()) {
        overlaped.back()->callback(context);
        if (!overlaped.back()->isOpen() || input.jactive(Escape))
            overlaped.pop_back();
        return; // Do not callback other containers.
    }
    for (const auto& it : containers) {
        it->callback(context);
    }
}

void GUI::addOverlaped(std::unique_ptr<Container> container) {
    container->translate(langTranslations);
    container->aplyAlignment(mainWindow.getSize());
    overlaped.push_back(std::move(container));
}

void GUI::acceptHotkeys() {
    if (input.jactive(Hide_GUI))
        showGUI = !showGUI;
    if (input.jactive(Screenshot)) {
        const std::string timeMs = std::to_string(util::time::getLocalTimeMilliseconds());
        mainWindow.takeScreenshot(io::folders::SCREENSHOTS / ("img" + timeMs + ".png"));
    }
    if (input.jactive(Show_FPS))
        showFPS = !showFPS;
    if (input.jactive(Show_atlas))
        showAtlas = !showAtlas;
    if (input.jactive(Show_hitboxes)) {
        Settings::gameplay.showHitboxes = !Settings::gameplay.showHitboxes;
        Settings::writeSettings();
    }
    if (input.jactive(Show_console)) {
        debug::Console::setVisible(!debug::Console::isVisible());
        Settings::gui.showConsole = debug::Console::isVisible();
        Settings::writeSettings();
    }
    if (input.jactive(Fullscreen)) {
        mainWindow.setFullscreen(!mainWindow.isFullscreen());
        Settings::display.fullscreen = mainWindow.isFullscreen();
        Settings::writeSettings();
    }
}

void GUI::relocateContainers() {
    for (const auto& it : containers) {
        it->aplyAlignment(mainWindow.getSize());
    }
    for (const auto& it : overlaped) {
        it->aplyAlignment(mainWindow.getSize());
    }
}

bool GUI::isMouseFree() const {
    if (!showGUI)
        return true;
    if (!overlaped.empty() && overlaped.back()->containsMouse(input))
        return false;
    for (const auto& it : containers) {
        if (it->containsMouse(input))
            return false;
    }
    return true;
}

bool GUI::ownsMouse() const {
    return hasOverlaped() || !isMouseFree();
}
