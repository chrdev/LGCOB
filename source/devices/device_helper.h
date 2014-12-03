/* devices/device_helper.h of LGCOB
 *
 * Common bit-operations to help the implementation of an input device.
 *
 * Inclusion of this file is optional, depending on whether you need these bit-operations.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#ifndef _DEVICE_HELPER_H_
#define _DEVICE_HELPER_H_

/* Some bit-operations borrowed from Arduino, renamed to conform to the project conventions. */
#define LowByte(w) ((uint8) ((w) & 0xff))
#define HighByte(w) ((uint8) ((w) >> 8))

#define BitRead(value, bit) (((value) >> (bit)) & 0x01)
#define BitSet(value, bit) ((value) |= (1UL << (bit)))
#define BitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define BitWrite(value, bit, bitvalue) (bitvalue ? BitSet(value, bit) : BitClear(value, bit))

#endif
