/****************************************************************************************
 *   FileName    : tc_color_templature.c
 *   Description : This is set templature/Gamma color using LUT Driver.
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

#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "tcc_lut_ioctl.h"

#include "tc_api_color_adjustment.h"
#include "tc_color_log.h"
#include "tc_color_util.h"
#include "tc_color_temperature.h"

#define TEMP_PREFIX	("COLOR")
#define TEMP_SUBPREFIX	("TEMP")

#define LUT_1D_DEVICE ("/dev/tcc_lut")
#define LUT_1D_SIZE (256U)
#define LUT_1D_DTYPE_SIZE (256.0)

static const mCLChar *lut_1d_drv_name = LUT_1D_DEVICE;

static mCLu32 TCColorCheckMinMaxColor(mCLDouble color)
{
	mCLu32 c;

	if(color >= 255.0) {
		c = 255U;
	}
	else if (color < 0.0) {
		c = 0U;
	}
	else {
		c = (mCLu32)color;
	}
	return c;
}

static mCLu32 TCColorRGBGround(mCLDouble color, mCLu32 pass_value)
{
	mCLu32 c;

	if (color >= 255.0) {
		c = 255U;
	}
	else if (color < (mCLDouble)pass_value) {
		c = pass_value;
	}
	else {
		c = (mCLu32)color;
	}

	return c;
}

static void TCColorMakeOriginalLUT(mCLu32 *table)
{
	mCLu32 i;
	mCLu32 mask = COLOR_MASK;

	if (table != NULL)
	{
		for (i = 0U; i < LUT_1D_SIZE; i++)
		{
			table[i] = ( ((i & mask) << 16U)
						| ((i & mask) << 8U)
						| (i & mask) );
		}
	}
}

static void TCColorMakeRevertedLUT(mCLu32 *table)
{
	mCLu32 i;
	mCLu32 mask = COLOR_MASK;
	mCLu32 value;

	if (table != NULL)
	{
		for (i = 0U; i < LUT_1D_SIZE; i++)
		{
			if (COLOR_MASK < i) {
				TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
					"Range Error i = %u\n", i);
			}
			else {
				value = COLOR_MASK - i;
				table[i] = ( ((value & mask) << 16U)
							| ((value & mask) << 8U)
							| (value & mask) );
			}
		}
	}
}

static mCLs32 TCColorGetUsefulKelvin(mCLs32 kelvin)
{
	mCLs32 cal_temperature;

	if (kelvin < 1000)
	{
		cal_temperature = 1000;
	}
	else if (kelvin > 40000)
	{
		cal_temperature = 40000;
	}
	else
	{
		cal_temperature = kelvin;
	}

	return cal_temperature;
}

static mCLDouble TCColorCalculateTemperatureRedFactor(mCLs32 temperature)
{
	mCLDouble color_factor;

	if (temperature <= 6600)
	{
		color_factor = 1.0;
	}
	else
	{
		color_factor = ((mCLDouble)temperature / 100.0);
		color_factor = (color_factor - 60.0);
		color_factor = 1.29293 * pow((color_factor), -0.13320);
	}

	return color_factor;
}

static mCLDouble TCColorCalculateTemperatureGreenFactor(mCLs32 temperature)
{
	mCLDouble color_factor;

	color_factor = ((mCLDouble)temperature / 100.0);

	if (temperature <= 6600)
	{
		color_factor = (0.39008 * log(color_factor)) - 0.63184;
	}
	else
	{
		color_factor = (color_factor - 60.0);
		color_factor = 1.129890 * pow(color_factor, -0.075514);
	}

	return color_factor;
}

static mCLDouble TCColorCalculateTemperatureBlueFactor(mCLs32 temperature)
{
	mCLDouble color_factor;

	if (temperature >= 6600)
	{
		color_factor = 1.0;
	}
	else if (temperature <= 1900)
	{
		color_factor = 0.0;
	}
	else
	{
		color_factor = ((mCLDouble)temperature / 100.0);
		color_factor = (color_factor - 10.0);
		color_factor = (0.54320 * log(color_factor)) - 1.19625;
	}

	return color_factor;
}

static mCLDouble TCColorCalcGamma(mCLDouble gamma_value, mCLDouble color_value)
{
	mCLDouble cac_value;
	mCLDouble lut_size = LUT_1D_DTYPE_SIZE;

	cac_value = ((lut_size - 1.0f) + 0.5f) * pow((color_value / lut_size), (1.0f / gamma_value));

	TCColorLog(TCColorLogLevelDebug, TEMP_PREFIX, TEMP_SUBPREFIX,
				"gamma: %f (%f to %f)",
				gamma_value, color_value, cac_value);

	return cac_value;
}

static void TCColorApplyGammaToLUT(mCLu32 *org_table, mCLu32 *table, RGBGAMMAINFO rgb_gamma_info, mCLu32 rgb_order)
{
	mCLu32 i;
	mCLu32 mask;
	mCLu32 org_red;
	mCLu32 org_green;
	mCLu32 org_blue;
	mCLu32 target_red;
	mCLu32 target_green;
	mCLu32 target_blue;

	mCLDouble gamma_red;
	mCLDouble gamma_green;
	mCLDouble gamma_blue;

	TCColorLog(TCColorLogLevelInfo, TEMP_PREFIX, TEMP_SUBPREFIX,
				"gamma: %f, %f, %f",
				rgb_gamma_info.red, rgb_gamma_info.green, rgb_gamma_info.blue);

	mask = COLOR_MASK;

	if ((org_table != NULL) && (table != NULL))
	{
		for (i = 0U; i < LUT_1D_SIZE; i++)
		{
			if (rgb_order == 1U)
			{
				org_red = (org_table[i] >> 16U) & mask;
				org_green = (org_table[i] >> 8U) & mask;
				org_blue = (org_table[i] & mask);
			}
			else
			{
				org_red = (org_table[i] & mask);
				org_green = (org_table[i] >> 8U) & mask;
				org_blue = (org_table[i] >> 16U) & mask;
			}

			gamma_red = TCColorCalcGamma(rgb_gamma_info.red, (mCLDouble)org_red);
			gamma_green = TCColorCalcGamma(rgb_gamma_info.green, (mCLDouble)org_green);
			gamma_blue = TCColorCalcGamma(rgb_gamma_info.blue, (mCLDouble)org_blue);

			target_red = TCColorRGBGround(gamma_red, rgb_gamma_info.r_ground);
			target_green = TCColorRGBGround(gamma_green, rgb_gamma_info.g_ground);
			target_blue = TCColorRGBGround(gamma_blue, rgb_gamma_info.b_ground);

			TCColorLog(TCColorLogLevelTable, TEMP_PREFIX, TEMP_SUBPREFIX,
						"(%4u, %4u, %4u) => (%4u, %4u, %4u)",
						org_red, org_green, org_blue,
						target_red, target_green, target_blue);

			if (rgb_order == 1U)
			{
				table[i] = ((target_red & mask) << 16U)
						| ((target_green & mask) << 8U)
						| (target_blue & mask);
			}
			else
			{
				table[i] = ((target_blue & mask) << 16U)
						| ((target_green & mask) << 8U)
						| (target_red & mask);
			}
		}
	}
}

static void TCColorMakeKelvinLUT(mCLu32 *org_table, mCLu32 *table, mCLs32 kelvin, mCLu32 rgb_order)
{
	mCLu32 i;
	mCLu32 mask = COLOR_MASK;
	mCLs32 cal_temp = TCColorGetUsefulKelvin(kelvin);
	mCLu32 org_red;
	mCLu32 org_green;
	mCLu32 org_blue;
	mCLu32 target_red;
	mCLu32 target_green;
	mCLu32 target_blue;

	mCLDouble red_factor;
	mCLDouble green_factor;
	mCLDouble blue_factor;

	TCColorLog(TCColorLogLevelInfo, TEMP_PREFIX, TEMP_SUBPREFIX,
				"kelvin: %d, cal_temp: %d", kelvin, cal_temp);

	if ((org_table != NULL) && (table != NULL))
	{
		for (i = 0U; i < LUT_1D_SIZE; i++)
		{
			if (rgb_order == 1U)
			{
				org_red = (org_table[i] >> 16U) & mask;
				org_green = (org_table[i] >> 8U) & mask;
				org_blue = (org_table[i] & mask);
			}
			else
			{
				org_red = (org_table[i] & mask);
				org_green = (org_table[i] >> 8U) & mask;
				org_blue = (org_table[i] >> 16U) & mask;
			}

			red_factor = TCColorCalculateTemperatureRedFactor(cal_temp);
			red_factor = (mCLDouble)org_red * red_factor;
			green_factor = TCColorCalculateTemperatureGreenFactor(cal_temp);
			green_factor = (mCLDouble)org_green * green_factor;
			blue_factor = TCColorCalculateTemperatureBlueFactor(cal_temp);
			blue_factor = (mCLDouble)org_blue * blue_factor;

			target_red = TCColorCheckMinMaxColor(red_factor);
			target_green = TCColorCheckMinMaxColor(green_factor);
			target_blue = TCColorCheckMinMaxColor(blue_factor);

			TCColorLog(TCColorLogLevelDebug, TEMP_PREFIX, TEMP_SUBPREFIX,
						"RGB : (%4u, %4u, %4u) => (%4u, %4u, %4u)",
						org_red, org_green, org_blue,
						target_red, target_green, target_blue);

			if (rgb_order == 1U)
			{
				table[i] = ((target_red & mask) << 16U)
						| ((target_green & mask) << 8U)
						| (target_blue & mask);
			}
			else
			{
				table[i] = ((target_blue & mask) << 16U)
						| ((target_green & mask) << 8U)
						| (target_red & mask);
			}
		}
	}
}

mCLs32 TCColorSetPlugin(mCLs32 fd, mCLu32 disp_num, mCLu32 plugin_num)
{
	mCLs32 ret;
	struct VIOC_LUT_PLUG_IN_SET plugin_set;

	if ((fd < 0) || (disp_num > DISP_DEV3) || (disp_num == 5U) || (disp_num == 6U))
	{
		ret = TC_COLOR_ERROR_ARGUMENT;
		TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
				"%s fd : %d, Disp_num : %u\n", ColorUtilGetErrorString(ret), fd, disp_num);
	}
	else
	{
		plugin_set.enable = 1;
		plugin_set.lut_number = disp_num;
		plugin_set.lut_plug_in_ch = plugin_num;
		ret = ioctl(fd, TCC_LUT_PLUG_IN, &plugin_set);

		if (ret < 0) {
			ret = ColorUtilGetErrorbyErrno((mCLs32)errno);
			TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
					"TCC_LUT_PLUG_IN Error : %s Disp(%u), Plugin(%u)\n",
					ColorUtilGetErrorString(ret), disp_num, plugin_num);
		}
	}

	return ret;
}

static mCLs32 TCColorUpdateLUT(mCLs32 fd, mCLu32 *table, mCLu32 disp_num)
{
	mCLu32 i;
	mCLs32 ret;
	struct VIOC_LUT_VALUE_SET_EX set_cmd;

	TCColorLog(TCColorLogLevelDebug, TEMP_PREFIX, TEMP_SUBPREFIX, "Update LUT Table\n");

	if ((fd < 0) || (table == NULL) || (disp_num > DISP_DEV3) || (disp_num == 5U) || (disp_num == 6U)) {
		ret = TC_COLOR_ERROR_ARGUMENT;
		TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX, "Argument Error!\n");
	}
	else {
		set_cmd.lut_number = disp_num;
		set_cmd.lut_size = LUT_1D_SIZE;
		set_cmd.param = 0;
		for (i = 0U; i < LUT_1D_SIZE; i++) {
			set_cmd.Gamma[i] = table[i];
		}

		ret = ioctl(fd, TCC_LUT_SET_EX, &set_cmd);
		if (ret < 0)
		{
			ret = ColorUtilGetErrorbyErrno(errno);
			TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
				"TCC_LUT_SET_EX Return : %s\n", ColorUtilGetErrorString(ret));
		}
	}

	return ret;
}

mCLs32 TCColorSetGammaUsingLUT(RGBGAMMAINFO rgb_gamma_info, mCLu32 disp_num, mCLu32 plugin_num)
{
	mCLs32 ret;
	mCLs32 fd;
	mCLu32 rgb_order;
	mCLu32 lut_table[LUT_1D_SIZE];
	mCLu32 org_lut[LUT_1D_SIZE];
	mCLu32 *target_lut;

	fd = open(lut_1d_drv_name, O_RDWR);
	if (fd < 0) {
		TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
			"%s device not found\n", lut_1d_drv_name);
		ret = TC_COLOR_ERROR_IO;
	}
	else {
		if ((disp_num == DISP_COMP0) || (disp_num == DISP_COMP1))
		{
			rgb_order = 1;
			(void)TCColorSetPlugin(fd, disp_num, plugin_num);
		}
		else {
			rgb_order = 0;
		}

		TCColorMakeOriginalLUT(org_lut);

		TCColorApplyGammaToLUT(org_lut, lut_table, rgb_gamma_info, rgb_order);
		target_lut = lut_table;

		ret = TCColorUpdateLUT(fd, target_lut, disp_num);
		if (ret < 0)
		{
			TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
				"UPDATE LUT : %d\n", ret);
		}
		(void)close(fd);
	}
	return ret;
}

mCLs32 TCColorSetTemperatureUsingLUT(mCLs32 kelvin, mCLu32 disp_num, mCLu32 plugin_num)
{
	mCLs32 ret;
	mCLs32 fd;
	mCLu32 rgb_order;
	mCLu32 lut_table[LUT_1D_SIZE];
	mCLu32 org_lut[LUT_1D_SIZE];
	mCLu32 *target_lut;

	fd = open(lut_1d_drv_name, O_RDWR);
	if (fd < 0) {
		TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
			"%s device not found\n", lut_1d_drv_name);
		ret = TC_COLOR_ERROR_IO;
	}
	else {
		if ((disp_num == DISP_COMP0) || (disp_num == DISP_COMP1))
		{
			rgb_order = 1;
			(void)TCColorSetPlugin(fd, disp_num, plugin_num);
		}
		else {
			rgb_order = 0;
		}

		if (kelvin >= 0)
		{
			TCColorMakeOriginalLUT(org_lut);
		}

		if (kelvin > 0)
		{
			TCColorMakeKelvinLUT(org_lut, lut_table, kelvin, rgb_order);
			target_lut = lut_table;
		}
		else if (kelvin < 0)
		{
			TCColorMakeRevertedLUT(lut_table);
			target_lut = lut_table;
		}
		else {
			target_lut = org_lut;
		}

		ret = TCColorUpdateLUT(fd, target_lut, disp_num);
		if (ret < 0)
		{
			TCColorLog(TCColorLogLevelError, TEMP_PREFIX, TEMP_SUBPREFIX,
				"UPDATE LUT : %d\n", ret);
		}
		(void)close(fd);
	}
	return ret;
}

