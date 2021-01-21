# fileTransfer
File transfer based on Thread Pool and socket

just create a repository that doesn't contain code

It is single-threaded, and I will add the thread pool later

makefile

to be continued!!!

客户端添加线程池
每个线程一个socket连接服务器--避免线程竞争
服务器用epoll接收connfd接收客户端数据
mmap是一个很好的用于文件传输的映射
pthread_getaffinity_np进行cpu绑定
makefile的使用(common、client、 server)

my new change for test