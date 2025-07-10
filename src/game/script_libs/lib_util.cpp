#include "script_libs.hpp"
//
#include <ANGEL_SCRIPT/angelscript.h>
//#include "scriptstdstring.h"
#include <iostream>

static void print() {
    std::cout << "works" << std::endl;
}

void script_libs::registerUtil(ScriptsHandler& scriptsHandler) {
    auto engine = scriptsHandler.getRaw();
    //RegisterStdString(engine);
    engine->RegisterGlobalFunction("void util_print()", asFunctionPtr(print), asCALL_CDECL);
}
