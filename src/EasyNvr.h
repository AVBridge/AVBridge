//
// Created by uma on 2023/6/5.
//

#ifndef EASYNVR_EASYNVR_H
#define EASYNVR_EASYNVR_H

#include "core/IPlugin.h"
#include "env.h"
#include "PluginManager.h"


class EasyNvr {
public:
    static EasyNvr &NewEasyNvr() {
        static EasyNvr instance;  // ���״ε���ʱ����ʵ��������ֻ֤��һ��ʵ��

        return instance;
    }

    void Run();

private:
    EasyNvr() {}  // ���캯��˽�л�����ֹ�ⲿʵ��������
    ~EasyNvr() {} // ����ѡ����˽������������ȷ�������޷�ͨ�� delete ����

    // ���ÿ������캯���͸�ֵ��������Է�ֹ����ʵ��
    EasyNvr(const EasyNvr &) = delete;

    EasyNvr &operator=(const EasyNvr &) = delete;
};


#endif //EASYNVR_EASYNVR_H
