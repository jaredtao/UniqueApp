#include "UniqueApp.h"
#include <QDebug>
UniqueApp::UniqueApp(QObject *parent) : QObject(parent)
{
    //创建系统信号量
    QSystemSemaphore sema("Tao Semaphore", 1, QSystemSemaphore::Open);
    //请求信号量，保证接下来的操作唯一
    sema.acquire();

    //共享内存, 退出时自动释放
    unimem.setKey("Tao Memory");

    bool hasRunning = false;
    //获取共享内存
    if (unimem.attach()) {
        //获取成功，说明已经有APP启动，并创建了共享内存
        hasRunning = true;
    } else {
        //获取不成功，说明没有APP启动。创建一个共享内存
        unimem.create(1);
        //初始化为0
        unimem.lock();
        auto p = static_cast<char *>(unimem.data());
        p[0] = 0;
        unimem.unlock();

        hasRunning = false;
    }

    //释放信号量
    sema.release();

    if (hasRunning) {
        //已经运行，则共享内存里面写1
        unimem.lock();
        auto p = static_cast<char *>(unimem.data());
        p[0] = 1;
        unimem.unlock();
        qWarning() << QStringLiteral("已经有一个App在运行，唤醒它，我退出");

        //退出
        exit(0);
    }
}

void UniqueApp::setNewAppCallback(std::function<void ()> callback)
{
    mCallback = callback;
    connect(&mTimer, &QTimer::timeout, this, &UniqueApp::onTimeout);
    //定时器，每隔一秒，检查共享内存
    mTimer.start(1 * 1000);
}

void UniqueApp::onTimeout()
{
    bool hasNewApp = false;
    unimem.lock();
    auto p = static_cast<char *>(unimem.data());
    if (p[0] == 1) {//有新的APP启动并写了共享内存.
        p[0] = 0; //重置回共享内存
        hasNewApp = true;   //设置flag
    }
    unimem.unlock();

    if (hasNewApp) {
        mCallback();
    }
}
