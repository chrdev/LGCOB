/* uart_print.c of LGCOB
 *
 * Print through UART to help debugging.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#include "uart_print.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stream.h>
#include <sink.h>

bool uart_write(const char * buffer, uint16 size)
{
    Sink sink = StreamUartSink();
    uint16 offset = SinkClaim(sink, size);
    uint8 * address;
    if (offset == 0xffff) return FALSE;
    address = SinkMap(sink);
    memcpy(address + offset, buffer, size);
    return SinkFlush(sink, size);
}

int uart_printf(const char * format, ...)
{
    const size_t BUFFER_SIZE = 80;
    
    int ret = 0;
    va_list aptr;
    static char * buffer = 0;
    
    if (!buffer) {
        /* lazy-initialize a buffer, reuse it and never free */
        buffer = (char *)malloc(BUFFER_SIZE);    
        if (!buffer) {
            const char * message = "*** insufficient memory to print ***\n";
            uart_write(message, strlen(message));
            return ret;
        }
    }
    
    va_start(aptr, format);
    ret = vsprintf(buffer, format, aptr);
    va_end(aptr);
    
    uart_write(buffer, ret);
    return ret;
}
