#include "scripting.hpp"
//
#include "ANGEL_SCRIPT/scriptstdstring.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include "engine/debug/logger.hpp"

static debug::Logger logger("scripts_handler");
static const std::filesystem::path main_as("res/scripts/main.as");

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
    mainLoop->Release();
    context->Release();
    engine->Release();
}

void ScriptsHandler::execute() const {
    context->Prepare(mainLoop);
    context->Execute();
}

static std::string loadScript() {
    std::ifstream fin(main_as);
    if (!fin.is_open()) {
        logger.error() << "Script file does not exist or is corrupted. File: " << main_as;
        return "";
    }

    std::stringstream ss;
    ss << fin.rdbuf();
    return ss.str();
}

void ScriptsHandler::load() {
    auto scriptModule = engine->GetModule("MainModule", asGM_ALWAYS_CREATE);
    scriptModule->AddScriptSection("MainScript", loadScript().c_str());
    scriptModule->Build();

    mainLoop = scriptModule->GetFunctionByDecl("void main()");
    if (!mainLoop)
        logger.error() << "Script function \"void main()\" not found.";
}

void ScriptsHandler::registerFunction(cString declAS, asSFuncPtr functionPtr) const {
    if (engine->RegisterGlobalFunction(declAS, functionPtr, asCALL_CDECL) < 0)
        logger.error() << "Failed to register function: " << declAS;
}
