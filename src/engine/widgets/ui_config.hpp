#pragma once

#define START_NAMESPACE_MINGUI namespace mingui {
#define END_NAMESPACE_MINGUI }
#define MINGUI using namespace mingui;

#define MINGUI_DISABLE_COPY_AND_MOVE(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete; \
    Class(Class&&) = delete; \
    Class& operator=(Class&&) = delete;
