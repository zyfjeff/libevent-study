/*
 *	检查事件类型
 *
 */ 

#include <event2/event.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

void cb_func(evutil_socket_t fd, short what, void *arg)                                   
{                                                        
         const char *data = arg;                                                         
         printf("Got an event on socket %d:%s%s%s%s [%s]\n",                                  
             (int) fd,                                                         
             (what&EV_TIMEOUT) ? " timeout" : "",                                           
             (what&EV_READ)     ? " read" : "",                                              
             (what&EV_WRITE)    ? " write" : "",                                             
             (what&EV_SIGNAL)   ? " signal" : "",                                            
             data);                                                         
} 




/*
void cb_func(evutil_socket_t fd,short what,void *arg)
{
	const char* data = arg;
	//打印事件的类型和传递过来的数据
printf("Got an event on socket %d:%s%s%s%s [%s]",(int)fd,(what & EV_TIMEOUT)? : "timeout" : " ";,(what&EV_READ)? : "read" : " ",(what&EV_WRITE)? : "write" : " ",(what&EV_SIGNAL)? : "signal" : " ",data);
}

*/
void main_loop(evutil_socket_t fd2)
{
	struct event *ev1,*ev2;
	struct timeval five_seconds = {5,0};
	struct event_base *base = event_base_new();
	//ev1 = event_new(base,fd1,EV_TIMEOUT|EV_READ|EV_PERSIST,cb_func,(char*)"Reading event");
	ev1 = evtimer_new(base,cb_func,(char*)"timeout event");

	ev2 = event_new(base,fd2,EV_READ|EV_WRITE|EV_PERSIST,cb_func,(char*)"writing event");
	event_add(ev1,&five_seconds);
	event_add(ev2,NULL);
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
