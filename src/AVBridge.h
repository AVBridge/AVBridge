// File: AVBridge.h
// Author: uma
// Date: 2023/6/5

#ifndef AVBridge_AVBridge_H
#define AVBridge_AVBridge_H

#include "PluginManager.h"

class AVBridge {
public:
    AVBridge() {
        //�ر�libhv����־���
        hlog_disable();
    }

    ~AVBridge();

    void Run();

    void ServeHttp();

    void ServeTcp(IPlugin *plugin, TcpServer *srv);

private:
    HttpService router;
    HttpServer server;
    PluginManager *pluginManager;
    std::map<std::string, TcpServer *> pluginTcpServers;
};

#endif // AVBridge_AVBridge_H
