/****************************************************************************************
 *	 FileName	 : tc_color_temerature.h
 *	 Description : COLOR Temperature/Gamma api Header file.
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

#ifndef TC_COLOR_TEMPERATURE_H
#define TC_COLOR_TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	mCLDouble red;
	mCLDouble green;
	mCLDouble blue;

	mCLu32	r_ground;
	mCLu32	g_ground;
	mCLu32	b_ground;
} RGBGAMMAINFO;

extern mCLs32 TCColorSetGammaUsingLUT(RGBGAMMAINFO rgb_gamma_info, mCLu32 disp_num, mCLu32 plugin_num);
extern mCLs32 TCColorSetTemperatureUsingLUT(mCLs32 kelvin, mCLu32 disp_num, mCLu32 plugin_num);

#ifdef __cplusplus
}
#endif

#endif	/* TC_COLOR_TEMPERATURE_H */

