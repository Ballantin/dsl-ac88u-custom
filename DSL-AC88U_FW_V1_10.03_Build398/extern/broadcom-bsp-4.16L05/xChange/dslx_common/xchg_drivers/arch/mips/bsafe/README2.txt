/*****************************************************************************
*
* This README provides a description of how the uHSM driver provided in the
* Broadcom Security Group uHSM SDK was integrated into the Phonex driver
* environment.
*
*****************************************************************************/


The SDK contains several items, including documentation, a core device driver,
a device driver API library, and an API test program. The source for these
items are contained in several directories, with some directories having
contents being required by more than item (e.g. include files). Hence, to
reduce the complexity req'd to integrate any future revisions of the SDK,
the decision was made to make as few changes as possible to the source files
and the directory hierarchy.

The SDK Software Reference document [1] and the README.txt [2] should be used
as references for the integration requirements.

The following files have been added to those received with the SDK and
are included in the same directory.

- README2.txt (this file)
- bsafe_app.mk (API library makefile for Phonex environment)
- bsafe_drv.mk (Core Device Driver makefile for Phonex environment)


A Phonex upper driver contains the SDK Core Device Driver described in [1],
and implements the necessary OS specific requirements, and is located at
xchg_drivers\phonex\vxworks\uhsm. The binding of the Phonex driver to the
SDK driver is done with a functional API defined for the SDK driver.
 
A Phonex lower driver has been implemented as the Hardware I/O Interface
described in [1], and is located in xchg_drivers\bcm1103\uhsm.
The binding the SDK Core Driver to the Phonex lower driver is statically
defined via a functional API defined in the SDK. At some point,
should probably change this to be dynamically defined as is done with
other Phonex drivers. To do this will require the development of
the functional equivalent to the bsafe_readwrite.c that is provided
as part of the SDK.

