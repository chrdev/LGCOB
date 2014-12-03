/* LGCOB.c of LGCOB
 *
 * LGCOB main.
 *
 * Copyright (c) 2014 chrdev
 *
 * This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.
 */
#include <connection.h>
#include <panic.h>
#include <stream.h>
#include <hid.h>
#include <source.h>
#include <bdaddr.h>

#include "uart_print.h"
#include "devices/device.h"

#define CREATE_TYPED_MESSAGE(msg_id) msg_id##_T * tmsg = (msg_id##_T *)msg

static const uint32 HID_MINOR_GAMEPAD = 0x08;
static const uint32 diac[] = {0x9e8b00, 0x9e8b33};
static const hid_config my_hid_config = {sizeof(sdp_service_record), sdp_service_record};
static const hid_connection_config my_hid_connection_config = 
{
    11250,    /* Latency (11.25ms) */
    FALSE     /* Don't use Guaranteed QOS for interrupt channel */
};
#define POWER_TABLE_ENTRIES (sizeof(power_table) / sizeof(lp_power_table))
static const lp_power_table power_table[]=
{
	/* mode,    	min_interval, max_interval, attempt, timeout, duration */
	{lp_sniff,		18,           20,			1,		 1,	      5},
	{lp_sniff,		22,           40,			1,		 16,	  30},
	{lp_sniff,		42,           112,			1,		 16,	  600},
	{lp_sniff,		114,          800,			1,		 16,	  0},
};
/* Sniff Subrating parameters used if remote device supports Sniff Subrating */
static const uint16 SSR_MAX_REMOTE_LATENCY = 72;    /* 72 slots (45ms) */
static const uint16 SSR_MIN_REMOTE_TIMEOUT = 18;    /* 18 slots (11.25ms) */
static const uint16 SSR_MIN_LOCAL_TIMEOUT  = 0;     /* 0 slots (0ms) */

enum
{
    BT_TASK_INTERNAL_BASE = 100,
    BT_TASK_INTERNAL_REPORT_IND,
    BT_TASK_INTERNAL_TOP
};

typedef enum
{
    bt_task_initializing,
    bt_task_connected
}bt_task_state;

/*
  --- an excerpt from message_.h ---
  typedef struct TaskData { void (*handler)(Task, MessageId, Message); } TaskData;
  typedef struct TaskData *Task;
*/
typedef struct
{
    TaskData        task;
    bdaddr          host_bdaddr;
    
    HID *           hid;
    HID_LIB *       hid_lib;
    Sink            sink; /* HID interrupt sink */
    
    bt_task_state   state;
}BT_TASK_T;

static BT_TASK_T bt_task;

static void bt_task_handler(Task task, MessageId id, Message msg)
{
    switch(id) {
        case BT_TASK_INTERNAL_REPORT_IND:
        {
            if (bt_task.state == bt_task_connected) {
                DeviceReport(bt_task.hid);
                MessageSendLater(task, BT_TASK_INTERNAL_REPORT_IND, 0, 5);
            }
            
            break;
        }
        case CL_INIT_CFM:
        {
            CREATE_TYPED_MESSAGE(CL_INIT_CFM);
            UART_PRINT(("CL_INIT_CFM success=0x%x, host_version=0x%x\n", tmsg->status, tmsg->version));
            
            if (tmsg->status != success) Panic();
            HidInit(task, &my_hid_config);
            
            break;
        }
        /*
          Received when the HID library has initialised.
        */
        case HID_INIT_CFM:
        {
            CREATE_TYPED_MESSAGE(HID_INIT_CFM);
            UART_PRINT(("HID_INIT_CFM status=0x%x\n", tmsg->status));
            
            if (tmsg->status != hid_init_success) Panic();
            bt_task.hid_lib = tmsg->hid_lib;

            PanicFalse(SourceConfigure(StreamHidSource(), VM_SOURCE_HID_BUTTONS, 0));
            PanicFalse(SourceConfigure(StreamHidSource(), VM_SOURCE_HID_WHEEL, HID_WHEEL_NONE));
            PanicFalse(SourceConfigure(StreamHidSource(), VM_SOURCE_HID_IDLE_MODE, 0));
            PanicFalse(SourceConfigure(StreamHidSource(), VM_SOURCE_HID_SAMPLE_RATE, 0));
            PanicFalse(SourceConfigure(StreamHidSource(), VM_SOURCE_MESSAGES, 0)); /* Do net generate MESSAGE_MORE_DATA */

            ConnectionWriteClassOfDevice(HID_MAJOR_DEVICE_CLASS | HID_MINOR_GAMEPAD);
            
            ConnectionSmSetSecurityMode(task, sec_mode2_service, hci_enc_mode_off);
            ConnectionSmRegisterIncomingService(protocol_l2cap, 0, secl_in_authentication);
            
            ConnectionWriteInquiryAccessCode(task, diac, 2);
            ConnectionSmSetSdpSecurityIn(TRUE);

            ConnectionSmDeleteAllAuthDevices(0);
            ConnectionWriteScanEnable(hci_scan_enable_inq_and_page);

            break;
        }
        /*
          Will receive this due to a call to ConnectionSmSetSecurityMode.
        */
        case CL_SM_SECURITY_LEVEL_CFM:
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_SM_SECURITY_LEVEL_CFM);
            UART_PRINT(("CL_SM_SECURITY_LEVEL_CFM success=0x%x\n", tmsg->success));
#endif
            break;
        }
        /* 
          Will received this due to a call to ConnectionWriteInquiryAccessCode.
          On success tmsg->status == hci_success
        */
        case CL_DM_WRITE_INQUIRY_ACCESS_CODE_CFM: 
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_DM_WRITE_INQUIRY_ACCESS_CODE_CFM);
            UART_PRINT(("CL_DM_WRITE_INQUIRY_ACCESS_CODE_CFM status=0x%x\n", tmsg->status));
#endif
            break;
        }
        /*
          Unsolicited informational message
        */
        case CL_DM_ACL_OPENED_IND:
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_DM_ACL_OPENED_IND);
            UART_PRINT(("CL_DM_ACL_OPENED_IND incoming=0x%x, status=0x%x\n", tmsg->incoming, tmsg->status));
#endif
            break;
        }
        /*
          Unsolicited informational message
        */
        case CL_DM_ACL_CLOSED_IND:
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_DM_ACL_CLOSED_IND);
            UART_PRINT(("CL_DM_ACL_CLOSED_IND status=0x%x\n", tmsg->status));
#endif
            break;
        }
        /* Will receive this when the remote host initiates a connect request */
        case CL_SM_REMOTE_IO_CAPABILITY_IND:
        {
            CREATE_TYPED_MESSAGE(CL_SM_REMOTE_IO_CAPABILITY_IND);
            UART_PRINT(("CL_SM_REMOTE_IO_CAPABILITY_IND, host_bdaddr saved\n"));

            bt_task.host_bdaddr = tmsg->bd_addr;
            
            break;
        }
        case CL_SM_IO_CAPABILITY_REQ_IND:
        {
            UART_PRINT(("CL_SM_IO_CAPABILITY_REQ_IND\n"));
            ConnectionSmIoCapabilityResponse(&bt_task.host_bdaddr,  cl_sm_io_cap_no_input_no_output, FALSE, TRUE, FALSE, 0, 0);
            
            break;
        }
        case CL_SM_AUTHENTICATE_CFM:
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_SM_AUTHENTICATE_CFM);
            UART_PRINT(("CL_SM_AUTHENTICATE_CFM status=0x%x, key_type=0x%x, bonded=0x%x\n", tmsg->status, tmsg->key_type, tmsg->bonded));
#endif
            break;
        }
        case CL_SM_AUTHORISE_IND:
        {
            CREATE_TYPED_MESSAGE(CL_SM_AUTHORISE_IND);
            UART_PRINT(("CL_SM_AUTHORISE_IND protocol=0x%x, channel=0x%x, incoming=0x%x\n", tmsg->protocol_id, tmsg->channel, tmsg->incoming));
            
            if (BdaddrIsSame(&bt_task.host_bdaddr, &(tmsg->bd_addr)))
                ConnectionSmAuthoriseResponse(&tmsg->bd_addr, tmsg->protocol_id, tmsg->channel, tmsg->incoming, TRUE);
            else
                ConnectionSmAuthoriseResponse(&tmsg->bd_addr, tmsg->protocol_id, tmsg->channel, tmsg->incoming, FALSE);
            
            break;
        }
        case HID_CONNECT_IND:
        {
            CREATE_TYPED_MESSAGE(HID_CONNECT_IND);
            UART_PRINT(("HID_CONNECT_IND\n"));
            HidConnectResponse(tmsg->hid, task, TRUE, &my_hid_connection_config);
            
            break;
        }
        /*
          Will receive this when the connection to the remote host has been setup or if it has failed.
        */
        case HID_CONNECT_CFM:
        {
            CREATE_TYPED_MESSAGE(HID_CONNECT_CFM);
            UART_PRINT(("HID_CONNECT_CFM status=0x%x\n", tmsg->status));
            
            if (tmsg->status == hid_connect_success) {
                bt_task.hid = tmsg->hid;
                bt_task.sink = tmsg->interrupt_sink;
                
                ConnectionSmSetTrustLevel(&bt_task.host_bdaddr, TRUE);
                ConnectionWriteScanEnable(hci_scan_enable_off);

                ConnectionReadClockOffset(task, bt_task.sink);
                ConnectionReadRemoteSuppFeatures(task, bt_task.sink);
                
                bt_task.state = bt_task_connected;
                MessageSendLater(task, BT_TASK_INTERNAL_REPORT_IND, 0, 1000);
            }

            break;
        }
        case CL_DM_LINK_SUPERVISION_TIMEOUT_IND:
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_DM_LINK_SUPERVISION_TIMEOUT_IND);
            UART_PRINT(("CL_DM_LINK_SUPERVISION_TIMEOUT_IND timeout=0x%x\n", tmsg->timeout));
#endif
            break;
        }
        /* Obsolete message, but some host may still send it, so return success  */
        case HID_SET_IDLE_IND:
        {
            CREATE_TYPED_MESSAGE(HID_SET_IDLE_IND);
            UART_PRINT(("HID_SET_IDLE_IND idle_rate=0x%x\n", tmsg->idle_rate));
            
            HidSetIdleResponse(tmsg->hid, hid_success);
            
            break;
        }
        case CL_DM_CLOCK_OFFSET_CFM:
        {
            CREATE_TYPED_MESSAGE(CL_DM_CLOCK_OFFSET_CFM);
            UART_PRINT(("CL_DM_CLOCK_OFFSET_CFM status=0x%x, clock_offset=0x%x\n", tmsg->status, tmsg->clock_offset));
            if (tmsg->status == hci_success)
                ConnectionWriteCachedClockOffset(&bt_task.host_bdaddr, tmsg->clock_offset);
            
            break;
        }
        case CL_DM_REMOTE_FEATURES_CFM:
        {
            CREATE_TYPED_MESSAGE(CL_DM_REMOTE_FEATURES_CFM);
            UART_PRINT(("CL_DM_REMOTE_FEATURES_CFM status=0x%x\n", tmsg->status));
            if (tmsg->status == hci_success) {
                ConnectionSetLinkPolicy(bt_task.sink, POWER_TABLE_ENTRIES, power_table);
                if (tmsg->features[2] & 0x0200) /* remote host supports Sniff Subrating */
                {
                    ConnectionSetSniffSubRatePolicy(bt_task.sink, SSR_MAX_REMOTE_LATENCY, SSR_MIN_REMOTE_TIMEOUT, SSR_MIN_LOCAL_TIMEOUT);
                }
            }
            
            break;
        }
        case CL_DM_SNIFF_SUB_RATING_IND:
        {
#ifdef UART_PRINT_ENABLED
            CREATE_TYPED_MESSAGE(CL_DM_SNIFF_SUB_RATING_IND);
            UART_PRINT(("CL_DM_SNIFF_SUB_RATING_IND status=0x%x, ", tmsg->status));
            UART_PRINT(("transmit_latency=0x%x, receive_latency=0x%x, ", tmsg->transmit_latency, tmsg->receive_latency));
            UART_PRINT(("remote_timeout=0x%x, local_timeout=0x%x\n", tmsg->remote_timeout, tmsg->local_timeout));
#endif
            break;
        }
        /* When receiving this message, we should make the devcie discoverable */
        case HID_DISCONNECT_IND:
        {
            UART_PRINT(("HID_DISCONNECT_IND\n"));
            bt_task.state = bt_task_initializing;
            ConnectionWriteScanEnable(hci_scan_enable_inq_and_page);
            
            break;
        }
        /* May never be sent by the remote host. */
        case HID_CONTROL_IND:
        {
            CREATE_TYPED_MESSAGE(HID_CONTROL_IND);
            UART_PRINT(("HID_CONTROL_IND operation=0x%x\n", tmsg->operation));
            switch (tmsg->operation) {
                case hid_control_op_suspend:
                    break;
                case hid_control_op_exit_suspend:
                    break;
                case hid_control_op_unplug:
                    break;
                default:
                    break;
            }
            
            break;
        }
        /* May never be sent by the remote host. */
        case HID_GET_REPORT_IND:
        {
            CREATE_TYPED_MESSAGE(HID_GET_REPORT_IND);
            uint32 report = 0x01000000;    /* Empty report with a Report ID of 1 */
            UART_PRINT(("HID_GET_REPORT_IND type=0x%x, id=0x%x, length=0x%x\n", tmsg->report_type, tmsg->report_id, tmsg->report_length));
            if (tmsg->report_type == hid_report_input && tmsg->report_id == 0x01 && tmsg->report_length == 4)
                HidGetReportResponse(tmsg->hid, hid_success, hid_report_input, 4, (uint8 *)&report);
            else
                HidGetReportResponse(tmsg->hid, hid_invalid_param, tmsg->report_type, 0, 0);
            
            break;
        }
        /* Should never happen. Ignore if it does happen. */
        case HID_SET_REPORT_IND:
        {
            CREATE_TYPED_MESSAGE(HID_SET_REPORT_IND);
            UART_PRINT(("HID_SET_REPORT_IND type=0x%x size=0x%x\n", tmsg->report_type, tmsg->size_report_data));
            HidSetReportResponse(tmsg->hid, hid_success);
            
            break;
        }
        /* Not likely to happen. */
        case HID_GET_PROTOCOL_IND:
        {
            CREATE_TYPED_MESSAGE(HID_GET_PROTOCOL_IND);
            UART_PRINT(("HID_GET_PROTOCOL_IND\n"));
            HidGetProtocolResponse(tmsg->hid, hid_success, hid_protocol_report);
            
            break;
        }
        /* Should never happen, why are we dealing with this? */
        case HID_SET_PROTOCOL_IND:
        {
            CREATE_TYPED_MESSAGE(HID_SET_PROTOCOL_IND);
            UART_PRINT(("HID_SET_PROTOCOL_IND type=0x%x\n", tmsg->protocol));
            switch (tmsg->protocol) {
                case hid_protocol_report:
                    HidSetProtocolResponse(tmsg->hid, hid_success);
                    break;
                default:
                    HidSetProtocolResponse(tmsg->hid, hid_unsupported);
                    break;
            }
            
            break;
        }
        /* Hopefully this will make LGCOB compatible with Bluetooth 2.0 */
        case CL_SM_PIN_CODE_IND:
        {
            CREATE_TYPED_MESSAGE(CL_SM_PIN_CODE_IND);
            UART_PRINT(("CL_SM_PIN_CODE_IND, host_bdaddr saved\n"));
            
            bt_task.host_bdaddr = tmsg->bd_addr;
            ConnectionSmPinCodeResponse(&bt_task.host_bdaddr, 4, (uint8 *)"0000");
                    
            break;
        }
        /* If the code goes here, we have missed something */
        default:
        {
            UART_PRINT(("* Message ID: 0x%x\n", id));
            
            break;
        }
    }
}

int main(void)
{
#ifdef UART_PRINT_ENABLED
    StreamUartConfigure(VM_UART_RATE_38K4, VM_UART_STOP_ONE, VM_UART_PARITY_NONE);
    UART_PRINT(("LGCOB 20141203\n"));
#endif
    
    DeviceInitIo();
    bt_task.state = bt_task_initializing;
    bt_task.task.handler = bt_task_handler;
    ConnectionInit(&bt_task.task);
    MessageLoop();
    return 0;
}
