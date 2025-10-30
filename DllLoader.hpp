#pragma once
#include <windows.h>
#include <stack>
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
            number execute(str name, std::stack<number>& st);
            bool isOperation(crStr name) const;
            bool isFunction(crStr name) const;
            size_t getPrecedence(crStr name) const;
            str getPluginsPath() const noexcept { return pluginsPath; }
            
            void setPluginsPath(crStr path) { pluginsPath = path; }
            void unloadPlugins();
    };
}
