#include "container.hpp"

void Container::aplyAlignment(const PixelCoord windowSize) {
    if (align == Align::none)
        return;
    //
    if ((align | Align::centre) == align)
        setPosition((windowSize - getSize()) / 2.0f);
    //
    if ((align | Align::left) == align)
        setPosition(PixelCoord(0.0f, getPosition().y));
    else if ((align | Align::right) == align)
        setPosition(PixelCoord(windowSize.x - getSize().x, getPosition().y));
    //  
    if ((align | Align::up) == align)
        setPosition(PixelCoord(getPosition().x, 0.0f));
    else if ((align | Align::down) == align)
        setPosition(PixelCoord(getPosition().x, windowSize.y - getSize().y));
    //
    arrange();
}
