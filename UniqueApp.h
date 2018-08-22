#ifndef UNIQUEAPP_H
#define UNIQUEAPP_H

#include <QObject>

#include <QSharedMemory>
#include <QSystemSemaphore>

#include <functional>
#include <QTimer>
class UniqueApp : public QObject {
    Q_OBJECT
public:
    explicit UniqueApp(QObject *parent = nullptr);
    void setNewAppCallback(std::function<void()> callback);
signals:

public slots:
    void onTimeout();
private:
    QSharedMemory unimem;
    std::function<void()> mCallback;
    QTimer mTimer;
};

#endif // UNIQUEAPP_H
