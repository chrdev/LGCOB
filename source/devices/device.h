/* devices/device.h of LGCOB
 *
 * Conditionally include header files for varieties of devices.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#ifdef DEVICE_SS
#include "ss.h"
#endif

/*
  Add your new device header here.
  
  #ifdef DEVICE_NEW
  #include "device_new.h"
  #endif
*/

/* Please always keep skeleton the last device */
#ifdef DEVICE_SKELETON
#include "skeleton.h"
#endif

#endif
