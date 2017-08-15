/*
 * Bsafe uHSM source code README.
 *
 * $Copyright Open Broadcom Corporation$      
 * $Id: README.txt,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

	BroadSafe[bsafe] -uHSM 

To Compile:
	if bsafe_vers.h is not present, change directory to 'shared' and 
	run  ./bsafe_vers.sh . This creates a version file.
 	run make 


Directories:
shared		- common header files, common between the kernel module and library.
			bsafe_common.h -  contains common definitions, 
					Domain: accross modules[sys], lib and applications
					This is a master header file which includes 
					/* include vers.h */
					#include <bsafe_vers.h>

					/* returnstatus returned by the chip */
					#include <bsafe_returncode.h>

					/* status returned by driver */
					#include <bsafe_status.h>

					/* devname */
					#include <bsafe_devname.h>
			Platform dependent port files:
			bsafe_devname.h	- contains the BSAFE_DEVICE_NAME 
					Domain: modules[sys], lib, applications
					Would be used by applications to open/connect to the driver.
					platform dependent defines can be present here, 
					Note this means the app should be compiled with these flags. 
			bsafe_osl.h	- contains the platform independent layer.
					Domain: modules[sys], lib
					Platform dependent defines can be present, 
					so both the sys and lib has to be compiled with these flags.


sys		- module and module io headers
			creates: bsafemod.o
			bsafe.h 	-  bsafe_uhsm data sheet to struct conversion.
			bsafe_regs.h	- bsafe_uhsm data sheet to glb register definitions.
			bsafe_io.h	- module to app io structs
			bsafe_context.h - context maintained across 
					the push and pop of commands to and from the chip.
			bsafe_keymgmt.h	- wrapper to manage key allocations based on the internal key location buckets.
			bsafe_core_if.h	- interface callback function definitions.
			bsafe_io.c 	- handles creating the input/output chip struct's[bsafe.h] from the module io struct's[bsafe_io.h]. 
			bsafe_misc.c 	- contains misc functions for keymgmt, context mgmt, error msg display,
			bsafe_readwrite.c - an read/write wrapper which implements .
				void bsafe_write_to_device(void * dev,unsigned char *ptr,int size);
				void bsafe_read_from_device(void * dev, unsigned char *ptr,int size);
			bsafe_sim.c	- an io wrapper which connects to the c model simulation using sockets.

lib		- a uhsm library  for use with applicatins
			creates libbsafeuhsm.so
			bsafe_uhsm_lib.h	- a uhsm  wrapper, and extra wrapper's for other io commands.
			bsafe_uhsm_lib.c	- implements bsafe_uhsm_lib.h
					converts api [bsafe_uhsm_lib.h] to module io structs[bsafe_io.h]

diag		- a simple diag written on top of lib api for testing lib control paths.
			creates bsafe_uhsm_lib_test
			bsafe_uhsm_lib_test.c - a test app using bsafe_uhsm_lib.h [Not of much use except testing control path]

OS-INTERFACE wrappers[bsafe_(xx).c]:
	bsafe_(xx).c has to implement probe,initialize,read,write,isr and also maintain the device specific 'dev_context'. 
	'dev_context' is taken as opaque [void *] for the ioctl commands and never interpreted by the bsafe_io.c. The 'dev_context' is passed back to the wrapper implemented read write calls.

OS dependent :
Fuctions to be implemented in bsafe_(xx).c like in the bsafe_(sim).c
	All wrapper calls have to call their appropriate bsafe_<calls> if present.
	bsafe_(xx)_initialize();
	bsafe_(xx)_open();
	bsafe_(xx)_ioctl();
	bsafe_(xx)_close();
	bsafe_(xx)_clean();
	bsafe_(xx)_isr_callback()	 
		theorically param has to be passed in and used as context.  
			Since there is only one global context for bsafe_uhsm it is used.
		The param arg is not used by bsafe_isr_callback.
	Note:
	* OS dependent locks have be implemented across the bsafe_(xx)_ioctl to ensure that only one command is in the context of the chip 
		
IO-bus/interface:
  	where 'dev_context' is 'void *' which is passed back  dev_context argument of  bsafe_ioctl 
	int bsafe_write_uint32(void * dev_context, int address, int *data_in);
	int bsafe_read_uint32(void * dev_context, int address, int *data_out);

Compiler FLAGS :
	LE_ENDIAN - little endian machine
	BE_ENDIAN - Big endian machine
	DEBUG - to enable debug
	BSAFE_SOCKET_SIM - this is for solaris socket simulation for pre-silicon testing
	BLOCK_IOCTL_MODE - a test mode for disabling blocking to test async mode.[should be enabled for current blocking mode.] [disabled mode not tested]
	BLOCK_LOOP_WITH_ISR - blocking mode for ioctl with isr mode. [not tested]

Documents:
	doc/*.pdf for API and porting guide ...


End of Document.
