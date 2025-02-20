#ifndef __COMMON_H__
#define __COMMON_H__



#define PRINTF_TAG "[Sys]"

#define USE_DEBUG_MODULE

#ifdef USE_DEBUG_MODULE
void rc_message(char *fmt, char *tag, ...);
void rc_message2(char *fmt, ...);
#define rc_printf(x,arg...) rc_message(x,PRINTF_TAG,##arg)
#define rc_printf2(x,arg...) rc_message2(x,##arg)

#else
#define rc_printf(x,arg...)
#define rc_printf2(x,arg...)

#endif


#endif
