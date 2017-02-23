#include <iostream>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string>
int main(int argc, char* argv[]) {
	int connfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientaddr.sin_port = htons(7788);

	if(connect(connfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr)) == -1) {
		std::cout<<"connect error"<<std::endl;
	}
   // char line[1000];
   // memset(line, '0', 1000);
    char* s = argv[1];
    int len = strlen(s);
   // strncpy(line, s, len);
    //line[len] = '\0';
    for(int i=0;i<10;i++) {
       send(connfd, s, len, 0);
       sleep(1);
    }
} 