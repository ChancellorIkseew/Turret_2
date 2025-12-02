#pragma once
#include <memory>

class Form;

class FormEditor {
public:
    static void init();
    static void update(const int frameDelay);
    static void drawCarriage();
    static void setForm(std::weak_ptr<Form> form);
private:
    static void resetTarget();
    static void editForm(const int frameDelay);
};
