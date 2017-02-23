#include <iostream>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "Scheduler.h"
#include "Coproc.h"

int set_nonblock(int fd)
{
 int flags = fcntl(fd, F_GETFL, 0);
 if (flags == -1)
  return -1;
 return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

class AcceptCo : public Coproc {
public:
    AcceptCo(int fd) : Coproc(fd) {}
    virtual void Run() {
        for(;;) {
            char line[1000];
            memset(line, '0', 1000);
            int num =0;
            if((num=read(fd_, line, 1000))<0) {
                if(errno == ECONNRESET) {
                    close(fd_);
                    break;
                }else {
                    printf("read error");
                }
            } else if(num==0) {
                close(fd_);      
                break;
            }
            line[num] = '\0';
            std::cout<<"read: "<<line<<std::endl;
            Yield();
        }
        Stop();
    }
};

class ServerCo : public Coproc {
public:
    ServerCo(int fd) : Coproc(fd) {}

    virtual void Run() {        
        for(;;) {
          std::cout<<"connection coming!"<<std::endl;
          int connfd = accept(fd_, NULL, NULL);
          set_nonblock(connfd);  
          Scheduler::getInstance()->PushCoproc(connfd, EPOLLIN, new AcceptCo(connfd)); //每一个连接开一个协程
          Yield(); //新连接到来，要让出主线程，不然要等到下个连接到了才会让
        }
    }
};


int main(void)
{
    struct sockaddr_in servaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblock(listenfd);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(7788);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    listen(listenfd, 10);

    ServerCo* server = new ServerCo(listenfd);
    //该描述符, 事件就绪时,执行后面的协程
    Scheduler::getInstance()->PushCoproc(listenfd, EPOLLIN, server);
    Scheduler::getInstance()->Poll();
    return 0;
}
