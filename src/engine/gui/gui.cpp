#include "gui.hpp"
//
#include "engine/debug/console.hpp"
#include "engine/engine.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"
#include "engine/settings/settings.hpp"
#include "engine/util/time.hpp"
#include "engine/window/window.hpp"
#include "t1_ui_context.hpp"
#include "t1_ui_renderer.hpp"

constexpr uint32_t BLACK = 0x00'00'00'FF;
constexpr PixelCoord DEBUD_PANEL_SIZE(200.f, 100.f);

GUI::GUI(Engine& engine) : engine(engine),
mainWindow(engine.getMainWindow()),
mainCanvas(mainWindow.getSize(), tin::read(io::folders::LANG / (Settings::gui.lang + ".tin")).release()),
input(mainWindow.getInput()) { }

static void drawDebugPanel(const Renderer& renderer, const MainWindow& mainWindow) {
    PixelCoord position = PixelCoord(mainWindow.getSize().x - DEBUD_PANEL_SIZE.x, 0.f);
    renderer.drawRect(BLACK, position, DEBUD_PANEL_SIZE);
    position += PixelCoord(20.f, 20.f);
    text::drawString(renderer, U"FPS|TPS: " + mingui::utf8::to_u32string(1000U / mainWindow.getRealFrameDelay()), position);
    position.y += 20.f;
    text::drawString(renderer, U"Frame|tick time: " + mingui::utf8::to_u32string(mainWindow.getRealFrameDelay()), position);
}

void GUI::draw(const Renderer& renderer, const Atlas& atlas) {
    if (mainWindow.justResized())
        mainCanvas.resize(mainWindow.getSize());
    //
    if (showGUI) {
        T1_UIRenderer uiRenderer(renderer);
        mainCanvas.draw(uiRenderer);
    }   
    if (showFPS)
        drawDebugPanel(renderer, mainWindow);
    if (showAtlas)
        renderer.drawFast(atlas.getComonTexture(), PixelCoord(0, 0), atlas.getSize());  
}

void GUI::translate(const std::string& lang) {
    tin::Data translation = tin::read(io::folders::LANG / (lang + ".tin"));
    mainCanvas.translate(translation.release());
}

void GUI::callback() {
    acceptHotkeys();
    if (!showGUI)
        return;
    T1_UIContext context(input, engine.getAssets().getAudio());
    mainCanvas.update(context, mainWindow.getRealFrameDelay());
    input.enableTextInput(mainCanvas.isTextEditingActive());
}

void GUI::addToOverlay(std::unique_ptr<mingui::Container> container) {
    mainCanvas.addToOverlay(std::move(container));
}

void GUI::acceptHotkeys() {
    if (input.jactive(Escape))
        mainCanvas.closeLastOverlaped();
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

bool GUI::ownsMouse() const {
    return mainCanvas.ownsMouse(input.getMouseCoord());
}
