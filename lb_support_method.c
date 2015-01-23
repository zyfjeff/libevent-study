#include <stdio.h>
#include <event2/event.h>

int main()
{
	int i = 0;
	const char **p = event_get_supported_methods();
	for(i =0;p[i] != NULL;i++)
		printf("%s\n",p[i]);
}
