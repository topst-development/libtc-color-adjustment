/****************************************************************************************
 *	 FileName	 : tc_api_color_adjustment.c
 *	 Description : This is function API of color adjustment using LUT.
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

#include "tc_api_color_adjustment.h"
#include "tc_color_util.h"
#include "tc_color_log.h"
#include "tc_color_hsv.h"
#include "tc_color_temperature.h"

#define API_PREFIX		("COLOR")
#define API_SUBPREFIX	("API")

static mCLs32 TCColorCheckHueSaturationArgument(mCLs32 angle, mCLs32 gain, mCLu32 disp_num)
{
	mCLs32 ret;

	if ((angle < -128) || (angle > 127)) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if ((gain < 0) || (gain > 1024)) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if (disp_num > 1U) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else {
		ret = TC_COLOR_NO_ERROR;
	}
	return ret;
}

void tc_api_color_initialize(void)
{
	TCColorLogInitialize();
}

void tc_api_set_log_level(mCLu32 log_level)
{
	TCColorLogSetLevel(log_level);
}

mCLs32 tc_api_color_set_hue_saturation(mCLs32 hue_angle, mCLs32 saturation_gain, mCLu32 disp_num)
{
	mCLs32 ret;
	mCLDouble gain_percent;

	ret = TCColorCheckHueSaturationArgument(hue_angle, saturation_gain, disp_num);
	if (ret != TC_COLOR_NO_ERROR) {
		TCColorLog(TCColorLogLevelError, API_PREFIX, API_SUBPREFIX,
			"Hue Saturation Argument Error! Hue(%d), Saturation(%d), disp(%u)\n",
			hue_angle, saturation_gain, disp_num);
	}
	else {
		gain_percent = (mCLDouble)saturation_gain/1024.0;

		ret = TCColorSetHueSaturationUsingLUT((mCLDouble)hue_angle, gain_percent, disp_num);
		if (ret != TC_COLOR_NO_ERROR) {
			TCColorLog(TCColorLogLevelError, API_PREFIX, API_SUBPREFIX,
				"Error : %s\n", ColorUtilGetErrorString(ret));
		}
	}
	return ret;
}

mCLs32 tc_api_color_on_off_3d_lut(mCLu32 onoff, mCLu32 disp_num)
{
	mCLs32 ret;
	ret = TCColorLUT3DOnOff(onoff, disp_num);
	return ret;
}

static mCLs32 TCColorCheckGammaArgument(mCLu32 gamma_red, mCLu32 gamma_green, mCLu32 gamma_blue,
	mCLu32 r_ground, mCLu32 g_ground, mCLu32 b_ground, mCLu32 disp_num, mCLu32 plugin_num)
{
	mCLs32 ret;

	if ((gamma_red < 300U) || (gamma_red > 2800U)) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if ((gamma_green < 300U) || (gamma_green > 2800U)) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if ((gamma_blue < 300U) || (gamma_blue > 2800U)) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if (r_ground > COLOR_MASK) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if (g_ground > COLOR_MASK) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if (b_ground > COLOR_MASK) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if (disp_num > DISP_DEV3) {
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else if ((disp_num == DISP_COMP0) || (disp_num == DISP_COMP1)) {
		if (plugin_num > PLUGIN_WDMA08) {
			ret = TC_COLOR_ERROR_ARGUMENT;
		}
		else {
			ret = TC_COLOR_NO_ERROR;
		}
	}
	else {
		ret = TC_COLOR_NO_ERROR;
	}
	return ret;
}

mCLs32 tc_api_color_set_gamma(mCLu32 gamma_red, mCLu32 gamma_green, mCLu32 gamma_blue,
	mCLu32 r_ground, mCLu32 g_ground, mCLu32 b_ground, mCLu32 disp_num, mCLu32 plugin_num)
{
	mCLs32 ret;
	RGBGAMMAINFO rgb_gamma;

	ret = TCColorCheckGammaArgument(gamma_red, gamma_green, gamma_blue,
			r_ground, g_ground, b_ground, disp_num, plugin_num);
	if (ret != TC_COLOR_NO_ERROR) {
		TCColorLog(TCColorLogLevelError, API_PREFIX, API_SUBPREFIX,
			"Gamma Argument Error! G.Red(%u), G.Green(%u), G.Blue(%u), Red.Ground(%u), Green.Ground(%u), Blue.Ground(%u), disp(%u)\n",
			gamma_red, gamma_green, gamma_blue,
			r_ground, g_ground, b_ground, disp_num);
	}
	else {
		rgb_gamma.red = (mCLDouble)gamma_red / 1000.0;
		rgb_gamma.green = (mCLDouble)gamma_green / 1000.0;
		rgb_gamma.blue = (mCLDouble)gamma_blue / 1000.0;
		rgb_gamma.r_ground = r_ground;
		rgb_gamma.g_ground = g_ground;
		rgb_gamma.b_ground = b_ground;

		ret = TCColorSetGammaUsingLUT(rgb_gamma, disp_num, plugin_num);
	}
	return ret;
}

mCLs32 tc_api_color_set_color_temperature(mCLs32 kelvin, mCLu32 disp_num, mCLu32 plugin_num)
{
	mCLs32 ret;
	ret = TCColorSetTemperatureUsingLUT(kelvin, disp_num, plugin_num);
	return ret;
}

