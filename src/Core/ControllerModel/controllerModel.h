#pragma once
#include <QObject>
#include <QThread>
#include "worker.h"

class Controller : public QObject {
    Q_OBJECT
    Q_PROPERTY(QObject* worker READ worker CONSTANT)
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    QObject* worker() const;

    void start();

private:
    QThread *workerThread;
    Worker* _worker;
};
