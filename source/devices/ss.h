/* devices/ss.h of LGCOB
 *
 * SEGA Saturn Gamepad Device File
 *
 * To add supprot for a new device, use this file as reference.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#ifndef _DEVICE_SS_H_
#define _DEVICE_SS_H_

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
  
  --- SDP Service Record for SEGA Saturn Gamepad ---
  To adapt to another devcie, we should modify the following four fields, and these fields only:
  
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
  0x25, 0x0c,        /* String length 12 */
  'S','S','G','a','m','e','p','a','d','-','B','T',
  
  0x09, 0x01, 0x01,  /* ServiceDescription */
  0x25, 0x22,        /* String length 34 */
  'S','E','G','A',' ','S','a','t','u','r','n',' ','G','a','m','e','p','a','d',' ','O','v','e','r',' ','B','l','u','e','t','o','o','t','h',
  
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
    0x29, 0x09,    /*   USAGE_MAXIMUM (Button 9) */
    0x15, 0x00,    /*   LOGICAL_MINIMUM (0) */
    0x25, 0x01,    /*   LOGICAL_MAXIMUM (1) */
    0x75, 0x01,    /*   REPORT_SIZE (1) */
    0x95, 0x09,    /*   REPORT_COUNT (9) */
    0x81, 0x02,    /*   INPUT (Data,Var,Abs) */
    0x75, 0x07,    /*   REPORT_SIZE (7) */
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

enum
{
    Y3  = 0x01,    /* Y3 form SSGamepad connected to PIO_00 */
    Y4  = 0x02,    /* Y4 form SSGamepad connected to PIO_01 */
    SB  = 0x04,    /* SB form SSGamepad connected to PIO_02 */
    SA  = 0x08,    /* SA form SSGamepad connected to PIO_03 */
    Y1  = 0x10,    /* Y1 form SSGamepad connected to PIO_04 */
    Y2  = 0x20     /* Y2 form SSGamepad connected to PIO_05 */
};

static const uint16 ALL_PIN_MASK = Y3 | Y4 | SB | SA | Y1 | Y2;
static const uint16 OUTPUT_PIN_MASK = SA | SB;

/* Setup PIO pins */
void DeviceInitIo(void)
{
    PioSetDir(ALL_PIN_MASK, OUTPUT_PIN_MASK);
}

/*
  On return, reading[] will be holding report data that can be sent to HID interrupt channel.
  
  reading[] should be 4 bytes in length to match with report size.
  
  reading[0] should be set to Report ID somewhere else and is not touched by this function
  
         --- Device Report Format ---
    Byte 0  |  Byte 1   |   Byte 2  |  Byte 3
  Report ID | Direction |  Buttons  |  Buttons
     0x01   | B0000YYXX | BRLZYXCBA | B0000000S
*/
static void DeviceRead(uint8 reading[])
{
    uint16 pio_reading;
    
    PioSet(OUTPUT_PIN_MASK, 0x00);
    pio_reading = ~PioGet();
    BitWrite(reading[2], 5, pio_reading & Y1);    /* Z */
    BitWrite(reading[2], 4, pio_reading & Y2);    /* Y */
    BitWrite(reading[2], 3, pio_reading & Y3);    /* X */
    BitWrite(reading[2], 7, pio_reading & Y4);    /* R */
    
    PioSet(OUTPUT_PIN_MASK, SA);
    pio_reading = ~PioGet();
    BitWrite(reading[2], 1, pio_reading & Y1);    /* B */
    BitWrite(reading[2], 2, pio_reading & Y2);    /* C */
    BitWrite(reading[2], 0, pio_reading & Y3);    /* A */
    BitWrite(reading[3], 0, pio_reading & Y4);    /* Start */
    
    PioSet(OUTPUT_PIN_MASK, SB);
    pio_reading = ~PioGet();
    BitWrite(reading[1], 3, pio_reading & Y1);    /* Up */
    BitWrite(reading[1], 2, pio_reading & Y2);    /* Down */
    BitWrite(reading[1], 1, pio_reading & Y3);    /* Left */
    BitWrite(reading[1], 0, pio_reading & Y4);    /* Right */
    
    PioSet(OUTPUT_PIN_MASK, SA | SB);
    pio_reading = ~PioGet();
    BitWrite(reading[2], 6, pio_reading & Y4);    /* L */
    
    /* 2's complement fix */
    if (reading[1] & 0x02) reading[1] |= 0x01;
    if (reading[1] & 0x08) reading[1] |= 0x04;
}

/* Send HID report */
void DeviceReport(HID * hid)
{
    static uint8 reading[4] = {0x01};
    static uint8 last_reading[4] = {0x01};
    bool report_updated = FALSE;
    int i;
    
    DeviceRead(reading);
    for (i = 1; i <= 3; ++i) {
        if (reading[i] != last_reading[i]){
            last_reading[i] = reading[i];
            report_updated = TRUE;
        }
    }
    if (report_updated) HidInterruptReport(hid, hid_report_input, 4, reading);
}

#endif
