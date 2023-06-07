//
// Created by uma on 2023/6/6.
//

#include "PluginRTMP.h"

Command PluginRTMP::React(std::any msg) {
    SPDLOG_INFO("PluginRTMP����");
    return Start;
}

PluginRTMP::PluginRTMP() {}


PluginRTMP::PluginRTMP(const char *name, const char *version, const char *author) {
    this->Name = name;
    this->Version = version;
    this->Author = author;
    this->HelloHttp();
    this->TcpServer();
}

void PluginRTMP::HelloHttp() {
    this->PluginHTTPMethod["hello"] = [this](HttpRequest *req, HttpResponse *resp) {
        return resp->String(this->Name);
    };
}

void PluginRTMP::TcpServer() {

    this->TcpServ.onMessage = [](const SocketChannelPtr &channel, Buffer *buf) {
        SPDLOG_INFO("len:{}", buf->size());
        PRINT_HEX(buf->data(), buf->size());
    };
    this->TcpServ.onConnection = [](const SocketChannelPtr &channel) {
        std::string peerAddr = channel->peeraddr();
        if (channel->isConnected()) {
            SPDLOG_INFO("{} connected! connfd={}", peerAddr.c_str(), channel->fd());
        } else {
            SPDLOG_INFO("{} disconnected! connfd={}", peerAddr.c_str(), channel->fd());
        }
    };
    this->TcpServ.port = 1935;
}

