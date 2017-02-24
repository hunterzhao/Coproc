#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <iostream>
#include <map>
#include <vector>
#include <utility> //pair
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <ucontext.h>
#include "common.h"
#define MAX_EVENTS 10

class Coproc;
DEFINE_SHARED_PTR(Coproc);
class Scheduler {
public:    
    //返回单例
    static Scheduler* getInstance() {
    	static Scheduler sche;
    	return &sche;
    }
    
    void PushCoproc(int fd, uint32_t event, CoprocPtr co);
    
    void DeleteCoproc(int fd);

    void DeleteCoproc(int fd, uint32_t event);

    CoprocPtr FindCoproc(int fd, uint32_t event);
    
    CoprocPtr GetCurrent();
    
    ucontext* GetMainCtx();

    void Poll();
    
private:
	Scheduler() {
		epollfd_ = epoll_create1(0);
	    if (epollfd_ == -1) {
	        perror("epoll_create1");
	        exit(EXIT_FAILURE);
	    }
	}
	Scheduler(const Scheduler&);
	Scheduler& operator=(const Scheduler& cur);

	//协程库
	std::map<int, std::vector<std::pair<uint32_t, CoprocPtr> > > coBuckets_;
	ucontext mainCtx_;
    CoprocPtr run_;

    int epollfd_;
    struct epoll_event events_[MAX_EVENTS];
};
#endif