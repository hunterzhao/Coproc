#ifndef COPROC_H_
#define COPROC_H_

#include <iostream>
#include <ucontext.h>

class Coproc {
public:
	static void pass(void* param);

	Coproc(int fd, int stack_size =  64 * 1024);

	~Coproc() {

	}
	//协程状态
	enum Status{
       READY,
       RUNNING,
       SUSPEND,
       STOP
	};
  
    //协程的执行内容
    virtual void Run() {
    	std::cout<<"virtual"<<std::endl;
    }
    
    int GetId() {
        return fd_;
    }

    ucontext_t *GetContext() {
    	return ctx_;
    }

    Status GetStatus() {
    	return st_;
    }

    void Yield();
    
    void Resume();

    void Stop() {
        st_ = STOP;
    }
    
    void Reset() {

    }

protected:
	 //与协程一一对应的描述符
	int fd_;

private:
	 //协程的上下文
    ucontext_t *ctx_;
     //协程当前的状态
    Status st_;
};

#endif