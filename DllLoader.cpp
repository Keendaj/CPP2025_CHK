#include "DllLoader.hpp"
#include <algorithm>
#include "utils/Exceptions/Math/MathException.hpp"


using namespace calculator;
namespace fs = std::filesystem;

DllLoader::DllLoader(crStr path) : pluginsPath(path) { loadPlugins(); }

DllLoader::~DllLoader() { unloadPlugins(); }


HMODULE DllLoader::loadAndCheckPlugin(crStr filepath) {
    HMODULE module = LoadLibraryA(filepath.c_str());
    if (!module) {
        return nullptr;
    }

    auto getNameFunc = (str(*)())GetProcAddress(module, "getName");
    auto isOperationFunc = (bool(*)())GetProcAddress(module, "isOperation");
    auto isFunctionFunc = (bool(*)())GetProcAddress(module, "isFunction");
    auto getPrecedenceFunc = (size_t(*)())GetProcAddress(module, "getPrecedence");

    if (!getNameFunc || !isOperationFunc || !isFunctionFunc || !getPrecedenceFunc) {
        FreeLibrary(module);
        return nullptr;
    }

    PluginData pd;
    pd.name = getNameFunc();
    pd.type = isFunctionFunc() ? OperationType::FUNCTION : OperationType::OPERATION;
    pd.precedence = getPrecedenceFunc ? getPrecedenceFunc() : 0;
    pd.path = filepath;
    pd.handle = module;

    data.push_back(pd);

    #ifdef TESTING
        std::cout << "Loaded plugin: " << pd.name 
        << " [" << (pd.type == OperationType::FUNCTION ? "FUNCTION" : "OPERATION") << "]" << std::endl;
    #endif

    return module;
}

void DllLoader::unloadPlugins() {
    for (auto& pd : data) {
        if (pd.handle) {
            FreeLibrary(pd.handle);
            pd.handle = nullptr;
        }
    }
    data.clear();
}


std::vector<str> DllLoader::findDllFiles() const {
    std::vector<str> dllFiles;
    
    try {
        if (!fs::exists(pluginsPath) || !fs::is_directory(pluginsPath)) {
            throw LoadException("Couldn't open plugins directory", pluginsPath);
        }
        
        for (const auto& entry : fs::directory_iterator(pluginsPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dll") {
                dllFiles.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        throw LoadException("Filesystem error: " + std::string(e.what()), pluginsPath);
    }
    
    if (dllFiles.empty()) {
        throw LoadException("No DLL files found in directory", pluginsPath);
    }
    
    return dllFiles;
}

bool DllLoader::loadPlugins() {
    unloadPlugins();

    auto dllFiles = findDllFiles();
    bool loadedAny = false;

    for (const auto& dllFile : dllFiles) {
        if(loadAndCheckPlugin(dllFile)){
            loadedAny = true;
        }
    }

    return loadedAny;
}

number DllLoader::execute(crStr name, std::stack<number>& st) {
    auto it = std::find_if(data.begin(), data.end(), [&](const PluginData& d) {
        return d.name == name;
    });

    if (it == data.end()) {
        throw DLLException("Coudln't find plugin: " + name);
    }

    auto calculateFunc = (void(*)(std::stack<number>&))GetProcAddress(it->handle, "getCalculation");

    if (!calculateFunc) {
        throw FunctionNotFoundException(it->name, "getCalculation", it->path);
    }
    try{
        calculateFunc(st);
    }
    catch(const std::exception& e){
        throw;
    }
    

    if (st.empty()) {
        throw std::runtime_error("Stack is empty after calculation");
    }

    return st.top();
}

bool DllLoader::isOperation(crStr name) const {
    auto it = std::find_if(data.begin(), data.end(),
        [&](const PluginData& pd) { return pd.name == name; });

    if (it == data.end()) {
        throw DLLException("Couldn't find plugin: " + name);
    }

    return it->type == OperationType::OPERATION;
}

bool DllLoader::isFunction(crStr name) const {
    auto it = std::find_if(data.begin(), data.end(),
        [&](const PluginData& pd) { return pd.name == name; });

    if (it == data.end()) {
        throw DLLException("Couldn't find plugin: " + name);
    }

    return it->type == OperationType::FUNCTION;
}

size_t DllLoader::getPrecedence(crStr name) const {
    auto it = std::find_if(data.begin(), data.end(),
        [&](const PluginData& pd) { return pd.name == name; });

    if (it == data.end()) {
        throw DLLException("Couldn't find plugin: " + name);
    }

    return it->precedence;
}