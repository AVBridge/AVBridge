//
// Created by uma on 2023/6/6.
//

#ifndef EASYNVR_PLUGINMANAGER_H
#define EASYNVR_PLUGINMANAGER_H


#ifdef _WIN32

#include <windows.h>

#else
#include <dlfcn.h>
#endif

#include "core/IPlugin.h"
#include "env.h"

class PluginManager {
public:
    PluginManager() {}

    ~PluginManager() {
        unloadAllPlugins();
    }


    IPlugin *loadPlugin(const std::string &pluginFileName);

    void unloadPlugin(const std::string &pluginName);

    IPlugin *getPluginByName(const std::string &pluginName);

private:

//    PluginManager() : plugins() {}



    // ���ÿ������캯���͸�ֵ��������Է�ֹ����ʵ��



    struct PluginData {
        void *pluginHandle;
        IPlugin *plugin;
        std::string pluginFileName;
    };

    std::map<std::string, PluginData> plugins;

    void unloadAllPlugins() {
        for (const auto &pluginData: plugins) {
            if (pluginData.second.plugin != nullptr) {
                delete pluginData.second.plugin;
            }

            // ж�ز��
#ifdef _WIN32
            FreeLibrary(static_cast<HMODULE>(pluginData.second.pluginHandle));
#else
            dlclose(pluginData.second.pluginHandle);
#endif
        }
    }
};


#endif //EASYNVR_PLUGINMANAGER_H
