/* devices/skeleton.h of LGCOB
 *
 * Skeleton Game Controller Device File
 *
 * To add support for a new device, copy this file as a start point, then see devices/ss.h for reference.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#ifndef _DEVICE_SKELETON_H_
#define _DEVICE_SKELETON_H_

/* Inclusion of <hid.h> is mandatory. */
#include <hid.h>
/* Inclusion of <pio.h> is optional, though we most likely need it. */
#include <pio.h>
/* Inclusion of "device_helper.h" is optional, depending on whether you need those bit-operations. */
#include "device_helper.h"

/* These two functions are mandatory. */
extern void DeviceInitIo(void);      /* Setup PIO pins */
extern void DeviceReport(HID * hid); /* Send HID report */

/*
  SDP Service Record is mandatory.
  
  --- Skeleton SDP Service Record ---
  To adapt to a new devcie, we should modify the following four fields, and these fields only:
  
  * ServiceName (0x0100)
  * ServiceDescription (0x0101)
  * ProviderName (0x0102)
  * HIDDescriptorList (0x0206)
*/
static const uint8 sdp_service_record[] =
{
  0x09, 0x00, 0x00,              /* ServiceRecordHandle */
  0x0a, 0x00, 0x01, 0x00, 0x01,  /* uint32 0x10001 */
  
  0x09, 0x00, 0x01,  /* ServiceClassIDList */
  0x35, 0x03,        /* DataElSeq 3 bytes */
  0x19, 0x11, 0x24,  /* uuid HID (0x1124) */
  
  0x09, 0x00, 0x04,  /* ProtocolDescriptorList */
  0x35, 0x0d,        /* DataElSeq 13 bytes */
  0x35, 0x06,        /* DataElSeq 6 bytes */
  0x19, 0x01, 0x00,  /* uuid L2CAP (0x0100) */
  0x09, 0x00, 0x11,  /* uint16 HID Control Channel (0x0011) */
  0x35, 0x03,        /* DataElSeq 3 bytes */
  0x19, 0x00, 0x11,  /* uuid HIDP (0x0011) */
  
  0x09, 0x00, 0x06,  /* LanguageBaseAttributeIDList */
  0x35, 0x09,        /* DataElSeq 9 bytes */
  0x09, 0x65, 0x6e,  /* uint16 0x656e */
  0x09, 0x00, 0x6a,  /* uint16 0x006a */
  0x09, 0x01, 0x00,  /* uint16 0x0100 */
      
  0x09, 0x00, 0x09,  /* BluetoothProfileDescriptorList */
  0x35, 0x08,        /* DataElSeq 8 bytes */
  0x35, 0x06,        /* DataElSeq 6 bytes */
  0x19, 0x11, 0x24,  /* uuid HID (0x1124) */
  0x09, 0x01, 0x01,  /* uint16 Version 1.1 (0x0101) */
         
  0x09, 0x00, 0x0d,  /* AdditionalProtocolDescriptorList */
  0x35, 0x0f,        /* DataElSeq 15 bytes */
  0x35, 0x0d,        /* DataElSeq 13 bytes */
  0x35, 0x06,        /* DataElSeq 6 bytes */
  0x19, 0x01, 0x00,  /* uuid L2CAP (0x0100) */
  0x09, 0x00, 0x13,  /* uint16 HID Interrupt Channel 0x0013 */      
  0x35, 0x03,        /* DataElSeq 3 bytes */
  0x19, 0x00, 0x11,  /* uuid HIDP (0x0011) */

  0x09, 0x01,0x00,   /* ServiceName */
  0x25, 0x08,        /* String length 8 */
  'S','k','e','l','e','t','o','n',
  
  0x09, 0x01, 0x01,  /* ServiceDescription */
  0x25, 0x08,        /* String length 8 */
  'S','k','e','l','e','t','o','n',
  
  0x09, 0x01, 0x02,  /* ProviderName */
  0x25, 0x06,        /* String length 6 */
  'c','h','r','d','e','v',
  
  0x09, 0x02, 0x01,  /* HIDParserVersion */
  0x09, 0x01, 0x11,  /* uint16 Version 1.11 (0x0111) */

  0x09, 0x02, 0x02,  /* HIDDeviceSubclass */
  0x08, 0x08,        /* uint8 Gamepad (0x08) */
    
  0x09, 0x02, 0x03,  /* HIDCountryCode */
  0x08, 0x00,        /* uint8 0x00 */
    
  0x09, 0x02, 0x04,  /* HIDVirtualCable */
  0x28, 0x01,        /* bool true */
    
  0x09, 0x02, 0x05,  /* HIDReconnectInitiate */
  0x28, 0x00,        /* bool false */
    
  0x09, 0x02, 0x06,  /* HIDDescriptorList */
  0x35, 0x39,        /* DataElSeq 57 bytes */
  0x35, 0x37,        /* DataElSeq 55 bytes */
  0x08, 0x22,        /* Descriptor Type: Report (code uint8 0x22) */
  0x25, 0x33,        /* HID Descriptor Length 51 */
    /* Start of HID Report Descriptor */
    0x05, 0x01,    /* USAGE_PAGE (Generic Desktop) */
    0x09, 0x05,    /* USAGE (Game Pad) */
    0xa1, 0x01,    /* COLLECTION (Application) */
    0x85, 0x01,    /*   REPORT_ID (1) */
    0x09, 0x30,    /*   USAGE (X) */
    0x09, 0x31,    /*   USAGE (Y) */
    0x15, 0xff,    /*   LOGICAL_MINIMUM (-1) */
    0x25, 0x01,    /*   LOGICAL_MAXIMUM (1) */
    0x75, 0x02,    /*   REPORT_SIZE (2) */
    0x95, 0x02,    /*   REPORT_COUNT (2) */
    0x81, 0x02,    /*   INPUT (Data,Var,Abs) */
    0x75, 0x04,    /*   REPORT_SIZE (4) */
    0x95, 0x01,    /*   REPORT_COUNT (1) */
    0x81, 0x01,    /*   INPUT (Cnst,Ary,Abs) */
    0x05, 0x09,    /*   USAGE_PAGE (Button) */
    0x19, 0x01,    /*   USAGE_MINIMUM (Button 1) */
    0x29, 0x0e,    /*   USAGE_MAXIMUM (Button 14) */
    0x15, 0x00,    /*   LOGICAL_MINIMUM (0) */
    0x25, 0x01,    /*   LOGICAL_MAXIMUM (1) */
    0x75, 0x01,    /*   REPORT_SIZE (1) */
    0x95, 0x0e,    /*   REPORT_COUNT (14) */
    0x81, 0x02,    /*   INPUT (Data,Var,Abs) */
    0x75, 0x02,    /*   REPORT_SIZE (2) */
    0x95, 0x01,    /*   REPORT_COUNT (1) */
    0x81, 0x01,    /*   INPUT (Cnst,Ary,Abs) */
    0xc0,          /* END_COLLECTION */
    /* End of HID Report Descriptor */
      
  0x09, 0x02, 0x07,  /* HIDLANGIDBaseList */
  0x35, 0x08,        /* DataElSeq 8 bytes */
  0x35, 0x06,        /* DataElSeq 6 bytes */
  0x09, 0x04, 0x09,  /* uint16 0x0409 */
  0x09, 0x01, 0x00,  /* uint16 0x0100 */

  0x09, 0x02, 0x09,  /* HIDBatteryPower */
  0x28, 0x00,        /* bool false */
    
  0x09, 0x02, 0x0a,  /* HIDRemoteWake */
  0x28, 0x01,        /* bool true */
    
  0x09, 0x02, 0x0c,  /* HIDSupervisionTimeout */ 
  0x09, 0x1f, 0x40,  /* uint16 8000(0x1f40)ms */

  0x09, 0x02, 0x0d,  /* HIDNormallyConnectable */
  0x28, 0x01,        /* bool true */
    
  0x09, 0x02, 0x0e,  /* HIDBootDevice */
  0x28, 0x00,        /* bool false */
  
  0x09, 0x02, 0x0f,  /* HIDSSRHostMaxLatency */
  0x09, 0x00, 0x48,  /* uint16, 72 slots, 45ms */
  
  0x09, 0x02, 0x10,  /* HIDSSRHostMinTimeout */
  0x09, 0x00, 0x12,  /* uint16, 18 slots, 11.25ms */
};

/* Setup PIO pins */
void DeviceInitIo(void)
{
    /* Write the implementation here. */
    /* Normally a call to PioSetDir() would suffice. */
    return;
}

/* Send HID report */
void DeviceReport(HID * hid)
{
    /* Write the implementation here. */
    /* If, and only if there is a reading change of the gamepad, we should call HidInterruptReport() to send out the report. */
    /* Be aware the report format should match with the HID Report Descriptor. */
    return;
}
#endif
