#pragma once
#include "MINGUI/core/ui_context_bridge.hpp"
#include "MINGUI/widgets/clickable.hpp"
#include "engine/audio/audio.hpp"
#include "engine/window/input/input.hpp"

class T1_UIContext : public mingui::UIContextBridge {
    Audio& audio;
public:
    ~T1_UIContext() final = default;
    T1_UIContext(Input& input, Audio& audio) : audio(audio) {
        const PixelCoord mouse = input.getMouseCoord();
        UIContextBridge::mousePosition = { mouse.x, mouse.y };
        UIContextBridge::mouseClicked = input.jactive(LMB);
    }

    void onIdled(mingui::Clickable& node) final {
        node.setColor(mingui::UIColor::idle);
    }
    void onHovered(mingui::Clickable& node) final {
        static const std::string UI_ON_HOVER = "ui_on_hover";
        audio.playUI(UI_ON_HOVER);
        node.setColor(mingui::UIColor::hover);
    }
    void onClicked(mingui::Clickable& node) final {
        /* nothings */
    }
    void onChecked(mingui::Clickable& node) final {
        node.setColor(mingui::UIColor::checked);
    }
};
