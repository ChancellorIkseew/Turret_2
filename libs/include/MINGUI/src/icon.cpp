#include <MINGUI/widgets/icon.hpp>
//
#include <MINGUI/render/render_bridge.hpp>

MINGUI

void Icon::draw(RenderBridge& renderBridge) {
    if (texture)
        renderBridge.drawSprite(Sprite(getPosition(), getSize(), texture.get()));
}
