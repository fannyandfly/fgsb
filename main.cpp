#include <QCoreApplication>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <pthread.h>
#include <SharedMemory.h>

int clientid;
SharedMemory MyShareMem;

void thread(void)
{

    printf("This is a thread.\n");

    while (true) {
        char buf[512];
        int len;
        //监听clientid端口的消息
        len = read(clientid,buf,512);
        buf[len] = 0;
        printf("data=%s\n",buf);
        //往客服端发送消息
        write(clientid,buf,len);
    }
}

static void* thread_callback(void *) {
    thread();
}

int main()
{
//    QCoreApplication a(argc, argv);
    int m_dServer;
    sockaddr_in m_Addr;
    m_dServer = socket(AF_INET, SOCK_STREAM, 0);
    if(m_dServer<0)
    {
        printf("Error!");
        return 0;
    }
    memset(&m_Addr, 0, sizeof(m_Addr));
    m_Addr.sin_family = AF_INET;
    m_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_Addr.sin_port = htons(5500);
    bind(m_dServer,(struct sockaddr*)&m_Addr,sizeof(m_Addr));
    if(listen(m_dServer,3)<0)
    {
        return 0;
    }

    int state;
    sockaddr sad;
    socklen_t sadlen = sizeof(sad);
    clientid = accept(m_dServer,&sad,&sadlen);
//    while(true)
//    {
        //创建进程
//        pid_t pr;
        pid_t tid = fork();
        if(tid == 0)//子进程中
        {
//            pid_t mid = getpid();//得到子进程PID
            close(m_dServer);

            pthread_t id;
            int ret;
            ret = pthread_create(&id, NULL, thread_callback, NULL);
            if(ret != 0){
                printf ("Create pthread error!n");
                exit (1);
            }
//            pthread_join(id,NULL);

//            while(true)
//            {
    /*          char buf[512];
                int len;
                //监听clientid端口的消息
                len = read(clientid,buf,512);
                buf[len] = 0;
                printf("data=%s\n",buf);
                //往客服端发送消息
                write(clientid,buf,len);*/
                pthread_join(id,NULL);
//            }
        }
        else//父进程中
        {
//            pr = wait(NULL);
            wait(&state);
            //printf("%d",getpid());//得到父进程PID
            close(clientid);
        }
//    }
//    exit(0);
//    return a.exec();
    return 0;
}
