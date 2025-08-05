#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"

std::unique_ptr<Container> frontend::initEditor() {
    auto editor = std::make_unique<Container>(Align::left | Align::down, Orientation::horizontal);
    auto frequancy = std::make_unique<Layout>(Orientation::vertical);
    auto deposite = std::make_unique<Layout>(Orientation::vertical);

    auto startWave = std::make_unique<Label>(U"frequancy");
    auto timeToWave = std::make_unique<Form>();

    frequancy->addNode(startWave.release());
    frequancy->addNode(timeToWave.release());


    auto f1 = std::make_unique<Label>(U"deposite");
    auto f2 = std::make_unique<Label>(U"text");

    deposite->addNode(f1.release());
    deposite->addNode(f2.release());

    editor->addNode(frequancy.release());
    editor->addNode(deposite.release());
    editor->arrange();
    return editor;
}
