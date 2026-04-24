#pragma once
#include "frontend.hpp"
//
#include <MINGUI/widgets/grid_layout.hpp>
#include <MINGUI/widgets/icon.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/gui.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "game/world/world.hpp"

class JEI;
enum class TileComponent : uint8_t { floor, overlay, block };
constexpr int ROW_SIZE = 6;
constexpr Point BTN_SIZE(32.0f, 32.0f);

struct TileData {
    TileComponent component = TileComponent::floor;
    uint8_t id = 0U;
    constexpr bool operator==(const TileData& other) const noexcept = default;
};

class JEISlot : public mingui::Icon {
    TileData tileData;
    JEI* jei;
    const Input& input;
public:
    JEISlot(const Point size, TextureBridge* texture, JEI* jei, const Input& input, TileData tileData) :
        Icon(size, texture), jei(jei), input(input), tileData(tileData) {
    }
    ~JEISlot() final = default;
    void callback(UIContext& context) final;
};

class JEI : public Container {
    Engine& engine;
    std::optional<TileData> optTileData;
    BlockRot rotation = up;
public:
    JEI(Engine& engine, JEIContent content) : Container(Align::right | Align::down, Orientation::vertical), engine(engine) {
        auto grid = addNode(new GridLayout(GridType::from_rows, ROW_SIZE));
        grid->setPalette(transparentPalette);
        const Input& input = engine.getMainWindow().getInput();

        if (content == JEIContent::all) {
            for (const auto& [floorName, id] : engine.getAssets().getIndexes().getFloor()) {
                addButton(floorName, id, TileComponent::floor, grid, input);
            }
            for (const auto& [oreName, id] : engine.getAssets().getPresets().getOres()) {
                addButton(oreName, id.asUint(), TileComponent::overlay, grid, input);
            }
        }
        for (const auto& [blockName, id] : engine.getAssets().getPresets().getBlocks()) {
            addButton(blockName, id.asUint(), TileComponent::block, grid, input);
        }
    }

    void addButton(const std::string& name, int id, TileComponent component, GridLayout* grid, const Input& input) {
        const Texture texture = engine.getAssets().getAtlas().at(name);
        grid->addNode(new JEISlot(BTN_SIZE, new T1_UITexture(texture), this, input, TileData(component, id)));
    }

    void callback(UIContext& context) override {
        Container::callback(context);
        const Input& input = engine.getMainWindow().getInput();
        if (input.jactive(Rotate_building))
            rotation = static_cast<BlockRot>((rotation + 1) % 4);
        if (!optTileData || !input.active(Build) || engine.getGUI().ownsMouse())
            return;
        const TileData tileData = optTileData.value();
        GameSession& session = engine.getSession();
        WorldMap& map = session.getWorld().getMap();
        const TileCoord tile = t1::tile(session.getCamera().fromScreenToMap(input.getMouseCoord()));
        switch (tileData.component) {
        case TileComponent::floor:   map.placeFloor(tile, tileData.id);   break;
        case TileComponent::overlay: map.placeOverlay(tile, OrePresetID(tileData.id)); break;
        case TileComponent::block: {
            const TeamID teamID = session.getPlayerController().getPlayerTeam()->getID();
            session.getBuiltInScripts().placeBlock(BlockPresetID(tileData.id), tile, teamID, rotation);
            break;
        }
        }
    }

    void setTileData(const TileData tileData) {
        if (optTileData && optTileData.value() == tileData)
            return optTileData.reset();
        optTileData = tileData;
    }

    void drawBlock(const Renderer& renderer, const PixelCoord mousePosition) {
        if (!optTileData)
            return;
        const BlockPreset& preset = engine.getAssets().getPresets().getBlock(BlockPresetID(optTileData.value().id));
        const Camera& camera = engine.getSession().getCamera();
        const PixelCoord size = preset.visual.size * camera.getMapScale();

        const TileCoord targetTile = t1::tile(camera.fromScreenToMap(mousePosition));
        const PixelCoord position = camera.fromMapToScreen(t1::pixel(targetTile));

        const bool canBuild = engine.getSession().getWorld().getBlocks().isAir(targetTile);

        if (canBuild) const_cast<Renderer&>(renderer).setColorModifier(255, 255, 255, 255);
        else          const_cast<Renderer&>(renderer).setColorModifier(180, 52, 52, 200);

        if (!preset.rotatable)
            renderer.drawFast(preset.visual.texture, position, size);
        else {
            const float angleRad = static_cast<float>(rotation) * t1::PI_F / 2.f;
            const PixelCoord origin = size / 2.f;
            renderer.draw(preset.visual.texture, position + origin, size, origin, angleRad);
        }
        const_cast<Renderer&>(renderer).resetColorModifier();
    }
};

void JEISlot::callback(UIContext& context){
    const bool clickedByPipette = context.containsMouse(*this) && input.jactive(Pipette);
    if (context.clicked(*this) || clickedByPipette)
        jei->setTileData(tileData);
}

std::unique_ptr<JEI> frontend::initJEI(Engine& engine, JEIContent content) {
    return std::make_unique<JEI>(engine, content);
}
