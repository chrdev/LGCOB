/* uart_print.h of LGCOB
 *
 * Print through UART to help debugging.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#ifndef _UART_PRINT_H_
#define _UART_PRINT_H_

#include <csrtypes.h>

/*
  Returns TRUE on success.
  Returns FALSE if the operation failed.
*/
bool uart_write(const char * buffer, uint16 size);

/*
  Returns the number of characters printed.
  CAUTION: Total string length should not exceed BUFFER_SIZE, see definition for details.
*/
int uart_printf(const char * format, ...);

#ifdef UART_PRINT_ENABLED
#define UART_PRINT(x) uart_printf x
#else
#define UART_PRINT(x)
#endif

#endif
