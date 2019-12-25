# UniqueApp

一个小Demo，实现了系统中只有一个APP实例。
启动第二个APP时，自动退出并将第一个APP置顶。

## 原理

```C++
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
```