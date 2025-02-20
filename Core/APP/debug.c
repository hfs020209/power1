#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "main.h"
#include "cmsis_os.h"
#include "common.h"
#include "Hal.h"

#define LOG_BUF (1024)

static char _log[LOG_BUF];
static volatile int lock;

void rc_message(char *fmt, char *tag, ...)
{
    va_list args;
    uint16_t len;

    if(lock)
        return;
    
    lock = 1;
    va_start (args, tag);
    memset(_log, 0, LOG_BUF);
    len = snprintf (_log, LOG_BUF, "%s", tag);
    len += vsnprintf (_log+len, LOG_BUF-len, fmt, args);
    HAL_UART_Transmit(&huart1, (void*)_log, len, HAL_MAX_DELAY);
    va_end (args);
    lock = 0;
}  




void rc_message2(char *fmt, ...)
{
    va_list args;
    uint16_t len;

    if(lock)
        return;
    
    lock = 1;
    va_start (args, fmt);
    memset(_log, 0, LOG_BUF);
    len = vsnprintf (_log, LOG_BUF, fmt, args);
    HAL_UART_Transmit(&huart1, (void*)_log, len, HAL_MAX_DELAY);
    va_end (args);
    lock = 0;
} 






































