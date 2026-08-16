#include "frontend.hpp"
//
#include <MINGUI/widgets/icon.hpp>
#include <MINGUI/widgets/icon_button.hpp>
#include "build_tools/build_tools.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/gui/t1_ui_renderer.hpp"

constexpr Point BTN_SIZE(32, 32);

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
        setPadding(6.f);
        
        const TextureRect textureRect = engine.getAssets().getAtlas().at("info_btn");
        addNode(new IconButton(PixelCoord(28, 28), 2, new T1_UITexture(textureRect)))->addCallback([&] { openBlockInfo(); });
        auto grid = addNode(new Layout(Orientation::horizontal_grid));
        grid->setCollRowLimit(6);
        grid->setPadding(0.f);
        grid->setPalette(NULL_PALETTE);

        const Input& input = engine.getMainWindow().getInput();
        if (buildTools->getContentLevel() == JEIContent::all) {
            for (const auto& [floorName, id] : engine.getAssets().getIndexes().getFloor()) {
                addButton(floorName, id, TileComponent::floor, input, grid);
            }
            for (const auto& [oreName, id] : engine.getAssets().getPresets().getOres()) {
                addButton(oreName, id.asUint(), TileComponent::overlay, input, grid);
            }
        }
        for (const auto& [blockName, id] : engine.getAssets().getPresets().getBlocks()) {
            addButton(blockName, id.asUint(), TileComponent::block, input, grid);
        }
    }

    void addButton(const std::string& name, int id, TileComponent component, const Input& input, Layout* grid) {
        const TextureRect textureRect = engine.getAssets().getAtlas().at(name);
        grid->addNode(new JEISlot(BTN_SIZE, new T1_UITexture(textureRect), buildTools.get(), input, TileData(component, id)));
    }

    void openBlockInfo() {
        if (buildTools->getTileData().has_value() && buildTools->getTileData()->component == TileComponent::block)
            engine.getGUI().addToOverlay(frontend::initBlockInfo(engine, buildTools->getTileData()->id));
    }
};

std::unique_ptr<Container> frontend::initJEI(Engine& engine, std::shared_ptr<BuildTools> buildTools) {
    return std::make_unique<JEI>(engine, buildTools);
}
