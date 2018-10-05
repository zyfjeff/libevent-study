# libevent-notes
Libevent notes

## 开启debug日志功能

```
#define EVENT_DBG_NONE 0
#define EVENT_DBG_ALL 0xffffffffu

void event_enable_debug_logging(ev_uint32_t which);
```



