#pragma once
#include <string>
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <stack>
#include "utils/Exceptions/DLL/DLLException.hpp"
#include "utils/Exceptions/DLL/LoadException.hpp"
#include "utils/Exceptions/DLL/FuncitonNotFoundException.hpp"
#include "PluginData.hpp"
#include "types.hpp"

namespace calculator{
    class DllLoader {
        
        private:
            str pluginsPath = "plugins";
            HMODULE currentPlugin = nullptr;
            str currentPluginName;
            
            HMODULE findAndLoadPlugin(const PluginData& data);
            HMODULE checkPlugin(crStr filepath, const PluginData& data);

            std::vector<str> findDllFiles() const;
        public:
            DllLoader() = default;
            explicit DllLoader(crStr path);
            ~DllLoader();

            bool load(const PluginData& data);
            number execute(const PluginData& data, std::stack<number>& st);

            bool isPluginLoaded() const { return currentPlugin != nullptr; }
            str getPluginsPath() const { return pluginsPath; }
            str getCurrentPluginName() const { return currentPluginName; }
            str getCurrentPluginPath() const;

            void setPluginsPath(crStr path);
            void unload();
    };
}
