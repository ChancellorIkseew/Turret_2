#include "scripting.hpp"
//
#include "ANGEL_SCRIPT/scriptstdstring.h"
#include <stdexcept>
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"

static debug::Logger logger("scripts_handler");

ScriptsHandler::ScriptsHandler() {
    engine = asCreateScriptEngine();
    if (!engine)
        throw std::runtime_error("Failed to create AngelScript engine.");

    context = engine->CreateContext();
    if (!context) {
        engine->Release();
        throw std::runtime_error("Failed to create context.");
    }
    RegisterStdString(engine);
}

ScriptsHandler::~ScriptsHandler() {
    context->Release();
    engine->Release();
}

void ScriptsHandler::execute() const {
    if (!mainLoop)
        return;
    context->Prepare(mainLoop);
    context->Execute();
}

void ScriptsHandler::load() {
    auto scriptModule = engine->GetModule("MainModule", asGM_ALWAYS_CREATE);
    scriptModule->AddScriptSection("MainScript", io::readFile("res/scripts/main.as").c_str());
    scriptModule->Build();

    mainLoop = scriptModule->GetFunctionByDecl("void main()");
    if (!mainLoop)
        logger.error() << "Script function \"void main()\" not found.";
}

void ScriptsHandler::registerFunction(cString declAS, asSFuncPtr functionPtr) const {
    if (engine->RegisterGlobalFunction(declAS, functionPtr, asCALL_CDECL) < 0)
        logger.error() << "Failed to register function: " << declAS;
}
