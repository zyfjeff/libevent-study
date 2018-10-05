#include <event2/event.h>
#include <string.h>

//检查字符串形式的版本号
int check_for_old_version(void)
{
      const char *v = event_get_version();
     	//检测是否是lbevent2.0之前的版本
	if (! strncmp(v, "0.", 2) ||
         ! strncmp(v, "1.1", 3) ||
         ! strncmp(v, "1.2", 3) ||
         ! strncmp(v, "1.3", 3)) {

         printf("Your version of Libevent is very old.If you run into bugs,"
                " consider upgrading.\n");
         return -1;
     } else {
         printf("Running with Libevent version %s\n", v);
         return 0;
     }
}
//检查数字形式的版本号
int check_version_match(void)
{
     ev_uint32_t v_compile, v_run;
     v_compile = LIBEVENT_VERSION_NUMBER; //编译时，通过#define定义的
     v_run = event_get_version_number();//运行时
     if ((v_compile & 0xffff0000) != (v_run & 0xffff0000)) {
         printf("Running with a Libevent version (%s) very different from the "
                "one we were built with (%s).\n", event_get_version(),
                LIBEVENT_VERSION);
         return -1;
     }
     return 0;
}
int main()
{
	check_for_old_version();
	check_version_match();
}
