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
        UIContextBridge::mousePosition = mingui::Point(input.getMouseCoord());
        UIContextBridge::mouseClicked = input.jactive(LMB);

        UIContextBridge::textInput.lastSymbolEntered = input.getLastSymbolEntered();
        UIContextBridge::textInput.delete_     = input.jactive(Delete);
        UIContextBridge::textInput.backspace_  = input.jactive(Backspace);
        UIContextBridge::textInput.arrowLeft_  = input.jactive(Arrow_left);
        UIContextBridge::textInput.arrowRight_ = input.jactive(Arrow_right);
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
