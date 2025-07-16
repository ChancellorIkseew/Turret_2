#include "script_libs.hpp"
//
#include "engine/debug/logger.hpp"

debug::Logger logger("scripts_lib_util");

static void print(const std::string& str) {
    logger.info() << str;
}

void script_libs::registerUtil(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("void util_print(string str)", asFunctionPtr(print));
}
