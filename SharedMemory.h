#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H
#include <QSharedMemory>
#include <QBuffer>
#include <QDataStream>
#include <stdio.h>

class SharedMemory : public QObject
{
    Q_OBJECT
public:
    SharedMemory(QObject *parent = 0);
    ~SharedMemory();
public slots:
    void loadIntoSharedMem();
    void loadFromSharedMem();
private:
    QSharedMemory sharedMem;
};
#endif // SHAREDMEMORY_H
