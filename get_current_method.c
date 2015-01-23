#include <event2/event.h>
#include <stdio.h>


void get_current_methodinfo(struct event_base *base)
{
	enum event_method_feature f;
	if(!base){
		printf("Could't get an event_base method info\n");
	}else {
		printf("Using Libevent with backend method %s.\n",event_base_get_method(base));
		
		f = event_base_get_features(base);
		if((f &EV_FEATURE_ET))
			printf("Edge-triggered events are support\n");
		if((f & EV_FEATURE_O1))
			printf("O(1) events are support\n");
		if((f & EV_FEATURE_FDS))
			printf("All FD types are support\n");
	}
}

int main()
{
	struct event_base *base = event_base_new();
	get_current_methodinfo(base);
}
