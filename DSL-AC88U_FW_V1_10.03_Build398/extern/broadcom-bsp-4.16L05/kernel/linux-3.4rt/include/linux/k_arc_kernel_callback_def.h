#ifndef _K_ARC_KERNEL_CALLBACK_DEF_H_
#define _K_ARC_KERNEL_CALLBACK_DEF_H_

/* Type definition for OSIX wrap, semaphore, message queue, task_id (process_id) */
typedef int      _tOsixThreadId;
typedef int      _tOsixQId;
typedef int      _tOsixSemId;
typedef int      _tOsixEventId;
typedef int      _tOsixTmrId;

typedef int (*_kOsixEventCrt_callback_type)(char *event_name, unsigned int mask_bits, unsigned int init_value,_tOsixEventId *eventid);
typedef int (*_kOsixEventDel_callback_type)(_tOsixEventId eventid);
typedef int (*_kOsixEventSend_callback_type)(_tOsixEventId eventid, unsigned int events);
typedef int (*_kOsixEventRecv_callback_type)(_tOsixEventId eventid, unsigned int *result, unsigned int timeout);
typedef int (*_kOsixEventObtainID_callback_type)(char *event_name, _tOsixEventId *eventid);

typedef int (*_kOsixQueCrt_callback_type)(char *qmsg_name, unsigned int msg_unit_size, unsigned int max_no_of_msgs, _tOsixQId *qid);
typedef int (*_kOsixQueDel_callback_type)(_tOsixQId qid);
typedef int (*_kOsixQueSend_callback_type)(_tOsixQId qid, char *pqmsg, unsigned int size);
typedef int (*_kOsixQueRecv_callback_type)(_tOsixQId qid, void *pqmsg, unsigned int size , int timeout);
typedef int (*_kOsixQueObtainID_callback_type)(char *qmsg_name, _tOsixQId *qid);

typedef int (*_kOsixSemCrt_callback_type)(char *sema_name, _tOsixSemId *sema_id, int init_count);
typedef int (*_kOsixSemDel_callback_type)(_tOsixSemId sema_id);
typedef int (*_kOsixSemTake_callback_type)(_tOsixSemId sema_id);
typedef int (*_kOsixSemGive_callback_type)(_tOsixSemId sema_id);
typedef int (*_kOsixSemObtainID_callback_type)(char *sema_name, _tOsixSemId *sema_id);

/* Export API Definition */
extern int _kOsixEventCrt_callback(char *event_name, unsigned int mask_bits, unsigned int init_value,_tOsixEventId *eventid);
extern int _kOsixEventDel_callback (_tOsixEventId eventid);
extern int _kOsixEventSend_callback (_tOsixEventId eventid, unsigned int events);
extern int _kOsixEventRecv_callback (_tOsixEventId eventid, unsigned int *result, unsigned int timeout);
extern int _kOsixEventObtainID_callback (char *event_name, _tOsixEventId *eventid);

extern int _kOsixQueCrt_callback(char *qmsg_name, unsigned int msg_unit_size, unsigned int max_no_of_msgs, _tOsixQId *qid);
extern int _kOsixQueDel_callback (_tOsixQId qid);
extern int _kOsixQueSend_callback(_tOsixQId qid, char *pqmsg, unsigned int size);
extern int _kOsixQueRecv_callback(_tOsixQId qid, void *pqmsg, unsigned int size , int timeout);
extern int _kOsixQueObtainID_callback(char *qmsg_name, _tOsixQId *qid);

extern int _kOsixSemCrt_callback(char *sema_name, _tOsixSemId *sema_id, int init_count);
extern int _kOsixSemDel_callback(_tOsixSemId sema_id);
extern int _kOsixSemTake_callback(_tOsixSemId sema_id);
extern int _kOsixSemGive_callback(_tOsixSemId sema_id);
extern int _kOsixSemObtainID_callback(char *sema_name, _tOsixSemId *sema_id);

extern void _kOsixEventRegister_callback(
	_kOsixEventCrt_callback_type _kOsixEventCrt_callback_fptr,
	_kOsixEventDel_callback_type _kOsixEventDel_callback_fptr,
	_kOsixEventSend_callback_type _kOsixEventSend_callback_fptr,
	_kOsixEventRecv_callback_type _kOsixEventRecv_callback_fptr,
	_kOsixEventObtainID_callback_type _kOsixEventObtainID_callback_fptr);

extern void _kOsixQueRegister_callback(
	_kOsixQueCrt_callback_type _kOsixQueCrt_callback_fptr,
	_kOsixQueDel_callback_type _kOsixQueDel_callback_fptr,
	_kOsixQueSend_callback_type _kOsixQueSend_callback_fptr,
	_kOsixQueRecv_callback_type _kOsixQueRecv_callback_fptr,
	_kOsixQueObtainID_callback_type _kOsixQueObtainID_callback_fptr);

extern void _kOsixSemRegister_callback(
	_kOsixSemCrt_callback_type _kOsixSemCrt_callback_fptr,
	_kOsixSemDel_callback_type _kOsixSemDel_callback_fptr,
	_kOsixSemTake_callback_type _kOsixSemTake_callback_fptr,
	_kOsixSemGive_callback_type _kOsixSemGive_callback_fptr,
	_kOsixSemObtainID_callback_type _kOsixSemObtainID_callback_fptr);


/* Generic Data Structure to Pass WAN Type Detection, Like xDSL Driver Detection, Ethernet WAN Detection.  */
/* Porting from ARC Common Utility */
#define		OSIX_NULL		(0)

#define		OSIX_TRUE		(1)
#define		OSIX_FALSE		(0)
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

typedef enum _OSIX_WRAPPER_ERRNO
{
	OSIX_SUCCESS = 0,
	OSIX_ERROR = -1,
	OSIX_RESOURCE_BUSY=-2,
	OSIX_TIMEOUT=-3,
}OSIX_WRAPPER_ERRNO;

#define OSIX_FAILURE   OSIX_ERROR

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
typedef enum{  
	WAN_PORT_REPORT_FROM_ADSL  = 0,  
	WAN_PORT_REPORT_FROM_VDSL,  
	WAN_PORT_REPORT_FROM_ETHERNET,
	WAN_PORT_REPORT_FROM_ADMIN
} WAN_DSL_MODE_T;

typedef struct WAN_PORT_LNK_DETECTION_STATUS{
	int line_status;
	struct timespec report_time; /* System Up Kernel Time */  
	unsigned char reporter;  /* WNA_PORT_REPORT_FROM_xDSL or WNA_PORT_REPORT_FROM_ETHERNET */
#if 0
		unsigned char reserved[3]; /* Reserved for the data structure padding */
#else
		unsigned char exception_state; /* exception_state to check xDSL sub line state */
		unsigned char reserved[2]; /* Reserved for the data structure padding */
#endif
}WAN_PORT_LNK_DETECTION_STATUS_T;

#define OSIX_MESSAGE_QUEUE_FOR_WAN_LINK_DETECTION_NAME      "MQUE_WAN"

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define GPIO_BUTTON_REPORT_FROM_KMODULE      0
#define GPIO_BUTTON_REPORT_FROM_WEB          1
#define GPIO_BUTTON_REPORT_FROM_TR69         2
#define GPIO_BUTTON_REPORT_FROM_TR64         3

#define GPIO_BUTTON_FACTORY_DEFAULT_BUTTON   32  /* Common Reset Pin */
#define GPIO_BUTTON_WLAN_BUTTON_PRESS        28  // Reserved UPKEY PUSH  // 33   /* WPS and DECT Registeration */
#define GPIO_BUTTON_PAGE_BUTTON_PRESS        54  // Reserved Downkey SELECT// 34  /* DECT Paging */
#define GPIO_BUTTON_DECT_REGISTER_BUTTON_PRESS         33 /* Same as WPS , OK KEY Select Reserved */


#define OSIX_GPIO_FACTORY_RESET_DETECTION_NAME    "MQ_RST"
#define OSIX_GPIO_WLAN_ADMIN_CTRL_DETECTION_NAME  "MQ_WADM"
#define OSIX_GPIO_WLAN_TIMER_DETECTION_NAME       "MQ_WLTM"
#define OSIX_GPIO_PAGING_DETECTION_NAME           "MQ_PAGE"
#define OSIX_GPIO_DECT_REGISTER_DETECTION_NAME    "MQ_DREG"

#define ARC_OSIX_SERVICE_THREAD_RUNNING      (1)
#define ARC_OSIX_SERVICE_THREAD_HALT            (0)

#define ARC_OSIX_EXECUTE_CMD_MAX_LENGTH    (512)

#define ARC_OSIX_SERVICE_SYSCMD_THREAD_STACK_SIZE   (8*1024)
typedef struct ARC_OSIX_EXECUTE_COMMAND_S{
	unsigned long execute_result;
	unsigned long request_pid;
	unsigned long foreground;  /* Execute them in the foreground [ under OSIXSCMD ] */
	char execute_cmd[ARC_OSIX_EXECUTE_CMD_MAX_LENGTH];
}ARC_OSIX_EXECUTE_COMMAND_T;

#define ARC_OSIX_SYSCMD_QUEUE_NAME   "OSIXSCMD"
#define ARC_OSIX_SYSCMD_MAX_QUEUE_NUMBER    (64)
#define ARC_OSIX_SYSCMD_MSG_QUE_UNIT_SIZE    sizeof(ARC_OSIX_EXECUTE_COMMAND_T)

typedef struct GPIO_PORT_BUTTON_DETECTION_STATUS{
	int trigger_gpio_pin;      /* GPIO_BUTTON_FACTORY_DEFAULT_BUTTON, GPIO_BUTTON_WLAN_BUTTON_PRESS, GPIO_BUTTON_PAGE_BUTTON_PRESS, GPIO_BUTTON_WPS_BUTTON_PRESS */
	unsigned char reporter;    /* WNA_PORT_REPORT_FROM_xDSL or WNA_PORT_REPORT_FROM_ETHERNET */
	unsigned char reserved[3]; /* Reserved for the data structure padding */
}GPIO_PORT_BUTTON_DETECTION_STATUS_T;

#define GPIO_BUTTON_MESSAGE_QUEUE_UNIT_SIZE sizeof(GPIO_PORT_BUTTON_DETECTION_STATUS_T)
#define GPIO_BUTTON_MESSAGE_QUEUE_NUMBER  64
#define WLAN_TIMER_MESSAGE_QUEUE_NUMBER   4

#endif
