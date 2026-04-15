#include "script_libs.hpp"

static void print(const std::string& str) {
    script_libs::logger.attention() << str;
}

void script_libs::registerUtil(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("void util_print(string str)", asFunctionPtr(print));
}
