# Coproc
A coroutine lib using c++ and ucontext epoll

>this lib can change the asynchronous IO to synchronous IO
>the scheduler is powered by epoll

example:
```
code...

the non-block IO call 
add the file descripter to the scheduler
yield() release the cpu and back when the fd ready

continue code... looks like synchronous way
```

the main file is the example of a tcpserver