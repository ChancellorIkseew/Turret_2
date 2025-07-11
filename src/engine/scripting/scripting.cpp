#include "scripting.hpp"
//
#include <ANGEL_SCRIPT/angelscript.h>
#include <stdexcept>

ScriptsHandler::ScriptsHandler() {
    engine = asCreateScriptEngine();
    if (!engine)
        throw std::runtime_error("Failed to create AngelScript engine.");

    context = engine->CreateContext();
    if (!context) {
        engine->Release();
        throw std::runtime_error("Failed to create context.");
    }
}

ScriptsHandler::~ScriptsHandler() {
    context->Release();
    engine->Release();
}

void ScriptsHandler::execute() const {
    context->Prepare(mainLoop);
    context->Execute();
}


#include <fstream>
#include <filesystem>
#include "engine/debug/logger.hpp"

static debug::Logger logger("scripts_handler");
static const std::filesystem::path main_as("res/scripts/main.as");

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
