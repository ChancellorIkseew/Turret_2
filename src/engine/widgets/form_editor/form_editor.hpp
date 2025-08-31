#pragma once

class Form;

class FormEditor {
public:
    static void setForm(Form* form);
    static void resetTarget();
    static void editForm();
};
