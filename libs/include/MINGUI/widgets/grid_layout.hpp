#pragma once
#include <MINGUI/widgets/layout.hpp>

START_NAMESPACE_MINGUI

class GridLayout : public Layout {
    int collRowLimit;
    Orientation contentsOrientation;
public:
    GridLayout(const GridType gridType, const int collRowLimit) :
        Layout(fromGridType(gridType)), collRowLimit(collRowLimit),
        contentsOrientation(fromGridTypeInverted(gridType)) {}
    virtual ~GridLayout() override = default;
    //
    void arrange() override;
    void draw(RenderQueue& queue) override;
private:
    static constexpr Orientation fromGridType(const GridType gridType) {
        if (gridType == GridType::from_colls)
            return Orientation::horizontal;
        return Orientation::vertical;
    }
    static constexpr Orientation fromGridTypeInverted(const GridType gridType) {
        if (gridType == GridType::from_colls)
            return Orientation::vertical;
        return Orientation::horizontal;
    }
protected:
    void emplaceNode(Node* node) override;
};

END_NAMESPACE_MINGUI
