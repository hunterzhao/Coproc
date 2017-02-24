
#include "Scheduler.h"
#include "Coproc.h"
void Scheduler::PushCoproc(int fd, uint32_t event, CoprocPtr co) {
    if(coBuckets_.find(fd) == coBuckets_.end())
    	coBuckets_[fd].push_back(std::make_pair(event, co));
    else
    	std::cout<<"already"<<std::endl;

    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl: error");
        exit(EXIT_FAILURE);
    }
}

void Scheduler::DeleteCoproc(int fd, uint32_t event) {
    auto it = coBuckets_.find(fd);
    if(it == coBuckets_.end())
        std::cout<<"no such fd"<<std::endl;
    
    coBuckets_.erase(it);
    //     coBuckets_[fd].push_back(std::make_pair(event, co));

    // struct epoll_event ev;
    // ev.events = event;
    // ev.data.fd = fd;
    // if(epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &ev) == -1) {
    //     perror("epoll_ctl: error");
    //     exit(EXIT_FAILURE);
    // }
}

void Scheduler::DeleteCoproc(int fd) {
    auto it = coBuckets_.find(fd);
    if(it == coBuckets_.end()) {
        std::cout<<"no such fd"<<std::endl;
        return;
    }
    coBuckets_.erase(it);
}

CoprocPtr Scheduler::FindCoproc(int fd, uint32_t event) {
	if(coBuckets_.find(fd) != coBuckets_.end()) {
		 auto container = coBuckets_[fd];
         for(auto data : container) 
            if(data.first & event)
                return data.second;
    } else {
    	 std::cout<<"no such one"<<std::endl;
    }
    return NULL;
}

CoprocPtr Scheduler::GetCurrent() {
	return run_;
}

ucontext* Scheduler::GetMainCtx() {
	return &mainCtx_;
}

void Scheduler::Poll() {
    //使用epoll判断当前执行哪个协程
    for(;;) {
        int nfds = epoll_wait(epollfd_, events_, MAX_EVENTS, -1);
        if(nfds == -1) {
            exit(EXIT_FAILURE);
        }

        for(int n = 0; n < nfds; n++) {
        	int fdtmp = events_[n].data.fd;
        	int eventtmp = events_[n].events;
            if(coBuckets_.find(fdtmp) != coBuckets_.end()) {
                auto container = coBuckets_[fdtmp];
                for(auto data : container) 
                	if(data.first & eventtmp) {
                        data.second->Resume();
                        break;
                    }
            }
            // }else if(events_[n].events & EPOLLIN) {
            //     int sockfd = events[i].data.fd;
            //     if(sockfd < 0) continue;
            //     //执行对应协程
            //     proc = map[sockfd];
            //     proc->Resume();
            //     // ev.data.fd = sockfd;
            //     // ev.events = EPOLLOUT | EPOLLET;
            //     // epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
            // }
            // }else if(events[n].events & EPOLLOUT) {
            //     int sockfd = events[n].data.fd;
            //      //执行对应协程
            //     proc = map[sockfd];
            //     swapcontext(&main_ctx, proc->ctx); //开始执行

            //     ev.data.fd = sockfd;
            //     ev.events = EPOLLIN | EPOLLET;
            //     epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
            // }     
        }
    }
    std::cout<< "here over" <<std::endl;
}