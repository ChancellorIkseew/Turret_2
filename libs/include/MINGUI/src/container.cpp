#include <MINGUI/widgets/container.hpp>

MINGUI

void Container::applyAlignment(const Point windowSize) {
    if (align == Align::none)
        return;
    //
    if ((align | Align::centre) == align)
        setPosition((windowSize - getSize()) / 2.0f);
    //
    if ((align | Align::left) == align)
        setPosition(Point(0.0f, getPosition().y));
    else if ((align | Align::right) == align)
        setPosition(Point(windowSize.x - getSize().x, getPosition().y));
    //
    if ((align | Align::up) == align)
        setPosition(Point(getPosition().x, 0.0f));
    else if ((align | Align::down) == align)
        setPosition(Point(getPosition().x, windowSize.y - getSize().y));
    //
    arrange();
}
