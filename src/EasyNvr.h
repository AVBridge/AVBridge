// File: EasyNvr.h
// Author: uma
// Date: 2023/6/5

#ifndef EASYNVR_EASYNVR_H
#define EASYNVR_EASYNVR_H

#include "PluginManager.h"

class EasyNvr {
public:
    EasyNvr() {}  // ���캯��˽�л�����ֹ�ⲿʵ��������
    ~EasyNvr() {
        SPDLOG_INFO("123");
    } // ����ѡ����˽������������ȷ�������޷�ͨ�� delete ����

    void Run();

    void ServeHttp();

    void ServeTcp(IPlugin *plugin, TcpServer *srv);

private:
    HttpService router;
    HttpServer server;
    std::map<std::string, TcpServer *> pluginTcpServers;
};

#endif // EASYNVR_EASYNVR_H
