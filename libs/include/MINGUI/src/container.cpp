#include <MINGUI/widgets/container.hpp>

MINGUI

void Container::applyAlignment(const Point canvasSize) {
    if (align == Align::none)
        return;
    //
    if ((align | Align::centre) == align)
        setPosition((canvasSize - getSize()) / 2.0f);
    //
    if ((align | Align::left) == align)
        setPosition(Point(0.0f, getPosition().y));
    else if ((align | Align::right) == align)
        setPosition(Point(canvasSize.x - getSize().x, getPosition().y));
    //
    if ((align | Align::up) == align)
        setPosition(Point(getPosition().x, 0.0f));
    else if ((align | Align::down) == align)
        setPosition(Point(getPosition().x, canvasSize.y - getSize().y));
    //
    arrange();
}
