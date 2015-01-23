#include <event2/event.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

void cb(int sock,short waht,void *arg)
{
//当有事件发生就会触发这个回调函数，这个回调函数就会调用event_base_loopbreak 退出当前循环
	struct event_base *base = arg;
	printf("server quiting\n");
	//event_base_loopexit(base,NULL);
	event_base_loopbreak(base);
}


void main_loop(struct event_base *base,evutil_socket_t watchdog_fd)
{
	struct event *watchdog_event;
	//创建一个事件，绑定fd到这个事件，设置回调函数
	watchdog_event = event_new(base,watchdog_fd,EV_READ,cb,base);
	//添加到事件集
	event_add(watchdog_event,NULL);
	event_base_dispatch(base);
}


//创建一个监听套接字
int MakeServer(int backlog)
{
        int ret;
        evutil_socket_t sockfd = socket(PF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
                ERR_EXIT("create socket");
        struct sockaddr_in addr;
        addr.sin_port = htons(81);
        addr.sin_family = AF_INET;
        if((ret = inet_pton(PF_INET,"0.0.0.0",&(addr.sin_addr))) < 0)
                ERR_EXIT("convert to ip");
        int value = 1;
        if((ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value))) < 0)
                ERR_EXIT("set socket");
        if((ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))) < 0)
                ERR_EXIT("bind socket");
        if((ret = listen(sockfd,backlog)) < 0)
                ERR_EXIT("listen socket");
        return sockfd;
}

int main()
{

	struct event_base *base = event_base_new();
	evutil_socket_t fd = MakeServer(100);
	main_loop(base,fd);
}
