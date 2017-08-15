::**************************************************************************
::
::     Copyright (c) 2005 Broadcom Corporation
::           All Rights Reserved
::
::     No portions of this material may be reproduced in any form without the
::     written permission of:
::
::           Broadcom Corporation
::           16215 Alton Parkway
::           P.O. Box 57013
::           Irvine, California 92619-7013
::
::     All information contained in this document is Broadcom Corporation
::     company private, proprietary, and trade secret.
::
::****************************************************************************

@echo off


::
:: Error check.
::

if not "%NATT_ENVIRONMENT_INITIALIZED%" == "1" goto oscheck
echo.
echo ******** ERROR: %TARGET_OS_TYPE% environment already initialized! ********
echo.
goto :eof

:oscheck
if "%1" == "VxWorks" goto start
if "%1" == "eCos" goto start
echo ******** ERROR: os type not specified ******
echo.
goto :eof

:start

:: Set target OS
::
:: set TARGET_OS_TYPE:=VxWorks
:: set TARGET_OS_TYPE:=eCos

set TARGET_OS_TYPE=%1

::
:: Set the root project directory.
::
cd ..\..\..
set SOURCE_PROJ_DIR=%CD:\=/%
cd prot_NATtrvrsl\xcbuild\test

::
:: Set the host platform - this is a batch file, so we must be on Windows.
::
set BUILD_HOST_TYPE=i386-win32

::
:: Add 'make', 'cygpath', location to the path.
::
set Path=%SOURCE_PROJ_DIR%/xchg_common/tools/bin/%BUILD_HOST_TYPE%



:: Set GCC Compiler sub-program path only works in unix path
:: set NATT_OS_ROOT_DIR=c:/cygwin

::
:: Tag the environment as initialized, and prevent the user from running this
:: script, or similar scripts in other projects again.
::
set NATT_ENVIRONMENT_INITIALIZED=1

:: Verbose mode
set verbose=1

::
:: Success!
::
echo.
echo === Successfully setup build environment for %TARGET_OS_TYPE% ===
echo.
