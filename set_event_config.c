#include <event2/event.h>
#include <stdio.h>

struct event_config *cfg;
struct event_base *base;
int i;

/* 可以通过event_config_require_features设置如下几种特性，
 * 设置边缘触发还是水平触发
 * 设置后端的事件添加、删除、和激活的时间复杂度是O(1)
 * 后端需要支持任意种事件类型，而不仅仅是socket套接字类型
enum event_method_feature {
    EV_FEATURE_ET = 0x01,
    EV_FEATURE_O1 = 0x02,
    EV_FEATURE_FDS = 0x04,
};
*/

/*
 * 可以通过event_base_config_flag设置如下几个类型
 * event_base不分配锁，可以在一定程度提高event_base的速度，但是在多线程环境下不安全
 * 让event_base在检测后端的方法时不检测EVENT_*的环境变量
 * event loop在每次准备去运行超时callback的时候都去检查一下当前时间，而不是用缓存值
 *
 enum event_base_config_flag {
    EVENT_BASE_FLAG_NOLOCK = 0x01,
    EVENT_BASE_FLAG_IGNORE_ENV = 0x02,
    EVENT_BASE_FLAG_STARTUP_IOCP = 0x04,
    EVENT_BASE_FLAG_NO_CACHE_TIME = 0x08,
    EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST = 0x10,
    EVENT_BASE_FLAG_PRECISE_TIMER = 0x20
};

event_config_set_max_dispatch_interval 设置最大的间隔，当每次event loop进行任务回调
的时候来检测高优先级的callback，默认每次都检测，通过设置这个间隔可以避免每次都进行任务回调

event_get_supported_methods 获取支持的后端列表
event_base_priority_init
event_base_get_method 获取指定base使用的后端方法


*/



void set_lb_config() {
	for (i=0; i<2; ++i) {
    // 创建event_config配置
    cfg = event_config_new();
    event_config_avoid_method(cfg, "select") ;
    if (i == 0)
      event_config_require_features(cfg, EV_FEATURE_ET);

    // 基于此配置创建event_base
    base = event_base_new_with_config(cfg);
    event_config_free(cfg);
    if (base)
          break;
  }
}

int main()
{
	set_lb_config();
}
