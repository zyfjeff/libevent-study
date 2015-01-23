/*
 *	设置event 优先级的
 *
 */ 

#include <event2/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

void cb_sockfunc(evutil_socket_t fd,short what,void *arg)
{
	printf("fd:%d\n",fd);
}

void cb_timerfunc(evutil_socket_t fd,short what,void *arg)
{
	printf("fd:%d\n",fd);
}


void main_loop(evutil_socket_t fd)
{
	struct event_base *base = event_base_new();
	event_base_priority_init(base,2);
	struct event *sockevent,*timerevent;	
	struct timeval twosecond = {2,0};
	sockevent = event_new(base,fd,EV_READ|EV_WRITE|EV_PERSIST,cb_sockfunc,NULL);
	timerevent = evtimer_new(base,cb_timerfunc,NULL);
	event_priority_set(sockevent,0);
	event_priority_set(timerevent,1);
	event_add(sockevent,NULL);
	event_add(timerevent,&twosecond);
	event_base_dispatch(base);
		
}

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
	evutil_socket_t fd = MakeServer(100);
	main_loop(fd);
}
