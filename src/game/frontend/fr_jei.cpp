#include "frontend.hpp"
//
#include <MINGUI/widgets/grid_layout.hpp>
#include <MINGUI/widgets/icon.hpp>
#include "build_tools/build_tools.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/gui/t1_ui_renderer.hpp"

constexpr int ROW_SIZE = 6;
constexpr Point BTN_SIZE(32.0f, 32.0f);

class JEISlot : public mingui::Icon {
    TileData tileData;
    BuildTools* tools;
    const Input& input;
public:
    JEISlot(const Point size, TextureBridge* texture, BuildTools* tools, const Input& input, const TileData tileData) :
        Icon(size, texture), tools(tools), input(input), tileData(tileData) {
    }
    ~JEISlot() final = default;
    //
    void callback(UIContext& context) final {
        const bool clickedByPipette = context.containsMouse(*this) && input.jactive(Pipette);
        if (context.clicked(*this) || clickedByPipette)
            tools->setTileData(tileData);
    }
};

class JEI : public Container {
    Engine& engine;
    std::shared_ptr<BuildTools> buildTools;
public:
    JEI(Engine& engine, std::shared_ptr<BuildTools> buildTools) : Container(Align::right | Align::down, Orientation::vertical),
        engine(engine), buildTools(buildTools) {
        auto grid = addNode(new GridLayout(GridType::from_rows, ROW_SIZE));
        grid->setPalette(transparentPalette);
        const Input& input = engine.getMainWindow().getInput();

        if (buildTools->getContentLevel() == JEIContent::all) {
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
        grid->addNode(new JEISlot(BTN_SIZE, new T1_UITexture(texture), buildTools.get(), input, TileData(component, id)));
    }

    void callback(UIContext& context) override {
        Container::callback(context);
        if (!engine.getGUI().ownsMouse())
            buildTools->update(engine);
    }
};

std::unique_ptr<Container> frontend::initJEI(Engine& engine, std::shared_ptr<BuildTools> buildTools) {
    return std::make_unique<JEI>(engine, buildTools);
}
