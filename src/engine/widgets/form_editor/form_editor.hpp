#pragma once
#include <memory>

class Input;
class Form;

class FormEditor {
public:
    static void init();
    static void update(Input& input, const int frameDelay);
    static void drawCarriage();
    static void setForm(const Input& input, std::weak_ptr<Form> form);
private:
    static void resetTarget();
    static void editForm(const Input& input, const int frameDelay);
    static void enableInput(Input& input);
    static void moveCarriageToCursor(const Input& input);
};
