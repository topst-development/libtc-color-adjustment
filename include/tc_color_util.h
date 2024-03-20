/****************************************************************************************
 *   FileName    : tc_color_util.h
 *   Description : COLOR utility function Header file.
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

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

#ifndef TC_COLOR_UTIL_H
#define TC_COLOR_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define TC_COLOR_NO_ERROR		(0)
#define TC_COLOR_ERROR_IO		(-5)
#define TC_COLOR_ERROR_ARGUMENT	(-22)
#define TC_COLOR_ERROR_NOMEM	(-12)
#define TC_COLOR_ERROR_BADFD	(-9)
#define TC_COLOR_ERROR_IOBUSY	(-16)
#define TC_COLOR_ERROR_UNKNOWN	(-99)

extern mCLs32 ColorUtilGetErrorbyErrno(mCLs32 value);
extern const mCLChar *ColorUtilGetErrorString(mCLs32 value);

#ifdef __cplusplus
}
#endif

#endif	/* TC_COLOR_UTIL_H */

