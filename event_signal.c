#include <event2/event.h>
#include <stdio.h>
#include <signal.h>

void sighup_function(evutil_socket_t fd,short what,void *arg)
{
	printf("I am is a signal\n");
}


int main()
{
	struct event *hup_event;                                                        
	struct event_base *base = event_base_new();                                                 
                                                                                            
/* call sighup_function on a HUP signal */
	hup_event = evsignal_new(base, SIGHUP, sighup_function, NULL);
	event_add(hup_event,NULL);
	
	event_base_dispatch(base);
}
