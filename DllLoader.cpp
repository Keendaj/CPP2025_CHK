#include "DllLoader.hpp"

using namespace calculator;
namespace fs = std::filesystem;

DllLoader::DllLoader(crStr path) : pluginsPath(path) {}

DllLoader::~DllLoader() { unload(); }

bool DllLoader::load(const PluginData& data) {
    try {
        HMODULE newPlugin = findAndLoadPlugin(data);
        if (newPlugin) {
            unload();
            currentPlugin = newPlugin;
            return true;
        }
    } catch (const std::exception& e) {
        throw LoadExecption("Failed to load plugin: " + std::string(e.what()), pluginsPath);
    }
    
    return false;
}

HMODULE DllLoader::findAndLoadPlugin(const PluginData& data) {
    auto dllFiles = findDllFiles();
    
    for (const auto& dllFile : dllFiles) {
        HMODULE candidate = checkPlugin(dllFile, data);
        if (candidate) {
            currentPluginName = dllFile;
            return candidate;
        }
    }
    
    throw DLLException("Couldn't find '" + data.name + "' with type: " + 
                      (data.type == OperationType::FUNCTION ? "FUNCTION" : "OPERATION"));
}

HMODULE DllLoader::checkPlugin(crStr filepath, const PluginData& data) {

    HMODULE module = LoadLibraryA(filepath.c_str());
    if (!module) {
        return nullptr;
    }

    auto getNameFunc = (str(*)())GetProcAddress(module, "getName");
    auto isOperationFunc = (bool(*)())GetProcAddress(module, "isOperation");
    auto isFunctionFunc = (bool(*)())GetProcAddress(module, "isFunction");

    if (!getNameFunc) {
        throw FunctionNotFoundException(currentPluginName, "getName", getCurrentPluginPath());
        FreeLibrary(module);
        return nullptr;
    }

    if (!isFunctionFunc) {
        throw FunctionNotFoundException(currentPluginName, "isFunction", getCurrentPluginPath());
        FreeLibrary(module);
        return nullptr;
    }

    if (!isOperationFunc) {
        throw FunctionNotFoundException(currentPluginName, "isOperation", getCurrentPluginPath());
        FreeLibrary(module);
        return nullptr;
    }

    if(getNameFunc() == data.name
        && (data.type == OperationType::FUNCTION 
            && isFunctionFunc())) {
                return module;
    }

    if (getNameFunc() == data.name
        && (data.type == OperationType::OPERATION 
            && isOperationFunc())) {
                return module;
    }

    FreeLibrary(module);
    return nullptr;
}

str DllLoader::getCurrentPluginPath() const {
    if (currentPluginName.empty()) {
        return "No plugin loaded";
    }
    return fs::path(currentPluginName).filename().string();
}

void DllLoader::setPluginsPath(crStr path) {
    if (isPluginLoaded()) {
        unload();
    }
    pluginsPath = path;
}

void DllLoader::unload() {
    if (currentPlugin) {
        FreeLibrary(currentPlugin);
        currentPlugin = nullptr;
        currentPluginName.clear();
    }
}

std::vector<str> DllLoader::findDllFiles() const {
    std::vector<str> dllFiles;
    
    try {
        if (!fs::exists(pluginsPath) || !fs::is_directory(pluginsPath)) {
            throw LoadExecption("Couldn't open plugins directory", pluginsPath);
        }
        
        for (const auto& entry : fs::directory_iterator(pluginsPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dll") {
                dllFiles.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        throw LoadExecption("Filesystem error: " + std::string(e.what()), pluginsPath);
    }
    
    if (dllFiles.empty()) {
        throw LoadExecption("No DLL files found in directory", pluginsPath);
    }
    
    return dllFiles;
}

number DllLoader::execute(const PluginData& data, std::stack<number>& st) {
    if (!isPluginLoaded() || getCurrentPluginName() != data.name) {
        if (!load(data)) {
            throw DLLException("Failed to load plugin: " + data.name);
        }
    }
    
    auto calculateFunc = (void(*)(std::stack<double>&))GetProcAddress(currentPlugin, "getCalculation");
    if (!calculateFunc) {
        throw FunctionNotFoundException(currentPluginName, "getCalculation", getCurrentPluginPath());
    }

    calculateFunc(st);
    
    if (st.empty()) {
        throw std::runtime_error("Stack is empty after calculation");
    }
    
    return st.top();
}