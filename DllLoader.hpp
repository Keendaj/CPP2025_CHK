#pragma once
#include <string>
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <stack>
#include <vector>
#include "utils/Exceptions/DLL/DLLException.hpp"
#include "utils/Exceptions/DLL/LoadException.hpp"
#include "utils/Exceptions/DLL/FuncitonNotFoundException.hpp"
#include "PluginData.hpp"
#include "types.hpp"

namespace calculator{
    class DllLoader {
        
        private:
            std::vector<PluginData> data;
            str pluginsPath = "plugins";
            HMODULE loadAndCheckPlugin(crStr filepath);

            std::vector<str> findDllFiles() const;
        public:
            explicit DllLoader(crStr path);
            ~DllLoader();

            bool loadPlugins();
            number execute(crStr name, std::stack<number>& st);
            bool isOperation(crStr name) const;
            bool isFunction(crStr name) const;
            size_t getPrecedence(crStr name) const;
            str getPluginsPath() const noexcept { return pluginsPath; }
            
            void setPluginsPath(crStr path) { pluginsPath = path; }
            void unloadPlugins();
    };
}
