//
// Created by uma on 2023/6/6.
//

#include "PluginManager.h"

IPlugin *PluginManager::loadPlugin(const std::string &pluginFileName) {
    // ���ز��
#ifdef _WIN32
    void *pluginHandle = LoadLibrary(pluginFileName.c_str());
#else
    void* pluginHandle = dlopen(pluginFileName.c_str(), RTLD_LAZY);
#endif

    if (pluginHandle == nullptr) {
#ifdef _WIN32
        SPDLOG_ERROR(Red("Failed to load the plugin ->{}<-."), pluginFileName);
#else
        SPDLOG_ERROR("���ز��ʧ�� {}",dlerror());
#endif
        return nullptr;
    }

    // ���庯��ָ������
    typedef IPlugin *(*Install)();

    // ��ȡ����ָ��
    Install install = nullptr;
#ifdef _WIN32
    install = reinterpret_cast<Install>(GetProcAddress(static_cast<HMODULE>(pluginHandle), "Install"));
#else
    install = reinterpret_cast<Install>(dlsym(pluginHandle, "Install"));
#endif

    if (install == nullptr) {
        SPDLOG_ERROR(Red("Failed to load the plugin interface ->{}<-."), pluginFileName);
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(pluginHandle));
#else
        dlclose(pluginHandle);
#endif
        return nullptr;
    }
    // ����������󲢱��浽�б���
    IPlugin *plugin = install();
    plugins[plugin->Name] = {pluginHandle, plugin};
    return plugin;
}

void PluginManager::unloadPlugin(const std::string &pluginName) {
    auto it = plugins.find(pluginName);
    if (it != plugins.end()) {
        if (it->second.plugin != nullptr) {
            delete it->second.plugin;
            it->second.plugin = nullptr;
        }
        // ж�ز��
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(it->second.pluginHandle));
#else
        dlclose(it->second.pluginHandle);
#endif

        plugins.erase(it);
    } else {
        SPDLOG_CRITICAL("δ�ҵ�ָ�����������ж��");
    }
}

IPlugin *PluginManager::getPluginByName(const std::string &pluginName) {
    auto it = plugins.find(pluginName);
    if (it != plugins.end()) {
        return it->second.plugin;
    }
    return nullptr;
}