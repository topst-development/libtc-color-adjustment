/****************************************************************************************
 *	 FileName	 : tc_color_util.c
 *	 Description : This is API function of color utility.
 ****************************************************************************************
 *
 *	 TCC Version 1.0
 *	 Copyright (c) Telechips Inc.
 *	 All rights reserved

This library contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information's accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "tc_api_color_adjustment.h"
#include "tc_color_util.h"

static const mCLChar *colorErrorStr[6] =
{
	"IO Error",
	"Argument Error",
	"No Memory",
	"Bad file descriptor",
	"IO Busy",
	"Unknown Error"
};

mCLs32 ColorUtilGetErrorbyErrno(mCLs32 value)
{
	mCLs32 ret;

	switch (value)
	{
		case EBADF:
			ret = TC_COLOR_ERROR_BADFD;
			break;
		case EFAULT:
			ret = TC_COLOR_ERROR_IO;
			break;
		case EINVAL:
			ret = TC_COLOR_ERROR_ARGUMENT;
			break;
		case ENOMEM:
			ret = TC_COLOR_ERROR_NOMEM;
			break;
		default:
			ret = TC_COLOR_ERROR_UNKNOWN;
			break;
	}
	return ret;
}

const mCLChar *ColorUtilGetErrorString(mCLs32 value)
{
	const mCLChar *ret;

	switch (value)
	{
		case TC_COLOR_ERROR_IO:
			ret = colorErrorStr[0];
			break;
		case TC_COLOR_ERROR_ARGUMENT:
			ret = colorErrorStr[1];
			break;
		case TC_COLOR_ERROR_NOMEM:
			ret = colorErrorStr[2];
			break;
		case TC_COLOR_ERROR_BADFD:
			ret = colorErrorStr[3];
			break;
		case TC_COLOR_ERROR_IOBUSY:
			ret = colorErrorStr[4];
			break;
		case TC_COLOR_ERROR_UNKNOWN:
		default :
			ret = colorErrorStr[5];
			break;
	}
	return ret;
}

