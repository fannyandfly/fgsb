#include <SharedMemory.h>

SharedMemory::SharedMemory( QObject *parent ) : QObject( parent ),sharedMem( "MySharedMemory" )
{

}

SharedMemory::~SharedMemory()
{

}

void SharedMemory::loadIntoSharedMem()
{
    if (sharedMem.isAttached())
    {
        sharedMem.detach();
    }
    QString text;
    scanf("%s",&text);
    if(text.length())
    {
        // load into shared memory
        QBuffer buffer;
        buffer.open(QBuffer::ReadWrite);
        QDataStream out(&buffer);
        out << text;
        int size = buffer.size();

        if (!sharedMem.create(size)) {
            printf("Unable to create shared memory segment.");
            return;
        }

        sharedMem.lock();
        char *to = (char*)sharedMem.data();
        const char *from = buffer.data().data();
        memcpy(to, from, qMin(sharedMem.size(), size));
        sharedMem.unlock();
    }
    else
    {
        printf("You must provide some texts");
    }
}

void SharedMemory::loadFromSharedMem()
{
    if (!sharedMem.attach())
    {
        printf("Unable to load!");
        return;
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    QString text;

    sharedMem.lock();
    buffer.setData((char*)sharedMem.constData(), sharedMem.size());
    buffer.open(QBuffer::ReadOnly);
    in >> text;
    sharedMem.unlock();

    sharedMem.detach();
    printf(text.toStdString().c_str());
}
