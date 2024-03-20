/****************************************************************************************
 *   FileName    : tc_color_hsv.c
 *   Description : This is function to set Hue Saturation gain using LUT 3D.
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "tcc_lut_3d_ioctl.h"

#include "tc_api_color_adjustment.h"
#include "tc_color_log.h"
#include "tc_color_util.h"
#include "tc_color_3d_tables.h"
#include "tc_color_hsv.h"

#define HSV_PREFIX	("COLOR")
#define HSV_SUBPREFIX	("HSV")

#define LUT_3D_DISP0_DRV	("/dev/tcc_lut_3d_d0")
#define LUT_3D_DISP1_DRV	("/dev/tcc_lut_3d_d1")

typedef struct {
	mCLDouble h;	// angle in degree [0, 360]
	mCLDouble s;	// [0, 1]
	mCLDouble v;	// [0, 1]
} COLORHSV;

typedef struct {
	mCLDouble r;	// [0, 1]
	mCLDouble g;	// [0, 1]
	mCLDouble b;	// [0, 1]
} COLORRGB;

#define MAX_LUT_3D_NUM	(2U)

static const mCLChar *lut_3d_drv_name[MAX_LUT_3D_NUM] = {
	LUT_3D_DISP0_DRV,
	LUT_3D_DISP1_DRV
};

static mCLDouble MaxColor(mCLDouble a, mCLDouble b, mCLDouble c);
static mCLDouble MinColor(mCLDouble a, mCLDouble b, mCLDouble c);
static mCLu32 CheckMinMaxColor(mCLDouble color);
static COLORRGB Covert2COLORRGB(mCLu32 value);
static mCLu32 Covert2COLOR(COLORRGB in);
static COLORHSV RGB2HSV(COLORRGB in);
static COLORHSV HSVPlusValue(COLORHSV in, mCLDouble angle, mCLDouble gain);
static COLORRGB HSV2RGB(COLORHSV in);

static mCLDouble MaxColor(mCLDouble a, mCLDouble b, mCLDouble c) {
   return ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c));
}

static mCLDouble MinColor(mCLDouble a, mCLDouble b, mCLDouble c) {
   return ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c));
}

static mCLu32 CheckMinMaxColor(mCLDouble color)
{
	mCLu32 c;

	if(color >= 255.0) {
		c = 255U;
	}
	else if (color < 0.0) {
		c = 0U;
	}
	else {
		c = (mCLu32)(color + 0.5);
	}
	return c;
}

static COLORRGB Covert2COLORRGB(mCLu32 value)
{
	// R, G, B values are divided by 255
	// to change the range from 0..255 to 0..1:
	COLORRGB out;
	mCLu32 r;
	mCLu32 g;
	mCLu32 b;
	mCLu32 mask;

	//value
	mask = COLOR_MASK;
	r = (value >> 16U) & mask;
	g = (value >> 8U) & mask;
	b = value & mask;

	out.r = (mCLDouble)r/255.0;
	out.g = (mCLDouble)g/255.0;
	out.b = (mCLDouble)b/255.0;

	TCColorLog(TCColorLogLevelDebug, HSV_PREFIX, HSV_SUBPREFIX,
		"RGB = (%u, %u, %u) \n", r, g, b);
	return out;
}

static mCLu32 Covert2COLOR(COLORRGB in)
{
	// to change the range from 0..1 to 0..255:
	mCLu32 out;
	mCLu32 r;
	mCLu32 g;
	mCLu32 b;
	mCLu32 mask;

	r = CheckMinMaxColor((in.r * 255.0));
	g = CheckMinMaxColor((in.g * 255.0));
	b = CheckMinMaxColor((in.b * 255.0));

	// Set Value
	mask = COLOR_MASK;
	out = ((r & mask) << 16U) | ((g & mask) << 8U) | (b & mask);

	TCColorLog(TCColorLogLevelDebug, HSV_PREFIX, HSV_SUBPREFIX,
		"R'G'B' = (%u, %u, %u) \n", r, g, b);
	return out;
}

static COLORHSV RGB2HSV(COLORRGB in)
{
	COLORHSV out;
	mCLDouble cmax;
	mCLDouble cmin;
	mCLDouble diff;
	mCLDouble temp;

	cmax = MaxColor(in.r, in.g, in.b); // maximum of r, g, b
	cmin = MinColor(in.r, in.g, in.b); // minimum of r, g, b
	diff = cmax-cmin; // diff of cmax and cmin.


	if (cmax == cmin) {
		out.h = 0.0;
	}
	else if (cmax == in.r) {
		temp = 60.0 * ((in.g - in.b) / diff);
		out.h = fmod((temp + 360.0), 360.0);
	}
	else if (cmax == in.g) {
		temp = 60.0 * ((in.b - in.r) / diff);
		out.h = fmod((temp + 120.0), 360.0);
	}
	else {
		// if cmax equal b
		temp = 60.0 * ((in.r - in.g) / diff);
		out.h = fmod((temp + 240.0), 360.0);
	}

	// if cmax equal zero
	if (cmax == 0.0) {
		out.s = 0.0;
	}
	else {
		out.s = (diff / cmax);
	}

	// compute v
	out.v = cmax;

	TCColorLog(TCColorLogLevelDebug, HSV_PREFIX, HSV_SUBPREFIX,
		"HSV = (%f, %f, %f)\n", out.h, out.s, out.v);
	return out;
}

static COLORHSV HSVPlusValue(COLORHSV in, mCLDouble angle, mCLDouble gain)
{
	COLORHSV out;
	mCLDouble h2;
	mCLDouble s2;

	h2 = in.h + angle;
	if (h2 >= 360.0) {
		out.h = h2 - 360.0;
	}
	else if (h2 < 0.0) {
		out.h = h2 + 360.0;
	}
	else {
		out.h = h2;
	}

	s2 = in.s + gain;
	if (s2 >= 1.0) {
		out.s = 1.0;
	}
	else if (s2 < 0.0) {
		out.s = 0.0;
	}
	else {
		out.s = s2;
	}

	out.v = in.v;

	TCColorLog(TCColorLogLevelDebug, HSV_PREFIX, HSV_SUBPREFIX,
		"H'S'V' = (%f, %f, %f)\n", out.h, out.s, out.v);
	return out;
}

static COLORRGB HSV2RGB(COLORHSV in)
{
	mCLDouble t;
	mCLDouble l;
	mCLDouble m;
	mCLDouble n;
	mCLDouble f;
	mCLLong i;
	COLORRGB out;

	if(in.s <= 0.0) {
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
	}
	else {
		t = in.h;
		if(t >= 360.0) {
			t = 0.0;
		}
		t /= 60.0;
		i = (mCLLong)t;
		f = t - (mCLDouble)i;
		l = in.v * (1.0 - in.s);
		m = in.v * (1.0 - (f * in.s));
		n = in.v * (1.0 - ((1.0 - f) * in.s));

		switch(i) {
			case 0:
				out.r = in.v;
				out.g = n;
				out.b = l;
				break;
			case 1:
				out.r = m;
				out.g = in.v;
				out.b = l;
				break;
			case 2:
				out.r = l;
				out.g = in.v;
				out.b = n;
				break;
			case 3:
				out.r = l;
				out.g = m;
				out.b = in.v;
				break;
			case 4:
				out.r = n;
				out.g = l;
				out.b = in.v;
				break;
			case 5:
			default:
				out.r = in.v;
				out.g = l;
				out.b = m;
				break;
		}
	}

	TCColorLog(TCColorLogLevelDebug, HSV_PREFIX, HSV_SUBPREFIX,
		"R'G'B'=(%f, %f, %f)\n", out.r, out.g, out.b);
	return out;
}

mCLs32 TCColorSetHueSaturationUsingLUT(mCLDouble angle, mCLDouble gain, mCLu32 disp)
{
	mCLs32 fd;
	mCLs32 ret;
	mCLu32 i;
	COLORRGB rgb;
	COLORRGB rgb2;
	COLORHSV hsv;
	COLORHSV hsv2;
	struct VIOC_LUT_3D_SET_TABLE set;

	if (disp >= MAX_LUT_3D_NUM)
	{
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else {
		fd = open(lut_3d_drv_name[disp], O_RDWR);
		if (fd < 0) {
			TCColorLog(TCColorLogLevelError, HSV_PREFIX, HSV_SUBPREFIX,
				"Can't open 3D LUT Device : %s\n", lut_3d_drv_name[disp]);
			ret = TC_COLOR_ERROR_IO;
		}
		else {
			for (i = 0U; i < LUT_3D_SIZE; i++)
			{
				rgb = Covert2COLORRGB(LUT_3D_ORIGINAL_TABLE[i]);
				hsv = RGB2HSV(rgb);
				hsv2 = HSVPlusValue(hsv, angle, gain);
				rgb2 = HSV2RGB(hsv2);
				set.table[i] = Covert2COLOR(rgb2);
			}

			TCColorLogTable(TCColorLogLevelTable, "LUT TABLE", set.table, LUT_3D_SIZE);

			ret = ioctl(fd, TCC_LUT_3D_SET_TABLE, &set);
			if (ret < 0) {
				ret = ColorUtilGetErrorbyErrno((mCLs32)errno);
				TCColorLog(TCColorLogLevelError, HSV_PREFIX, HSV_SUBPREFIX,
					"Can't set 3D LUT TABLE : %s\n", ColorUtilGetErrorString(ret));
			}
			(void)close(fd);
		}
	}
	return ret;
}

mCLs32 TCColorLUT3DOnOff(mCLu32 onoff, mCLu32 disp)
{
	mCLs32 ret;
	mCLs32 fd;
	struct VIOC_LUT_3D_ONOFF on_off;

	if (disp >= MAX_LUT_3D_NUM)
	{
		ret = TC_COLOR_ERROR_ARGUMENT;
	}
	else {
		fd = open(lut_3d_drv_name[disp], O_RDWR);
		if (fd < 0) {
			TCColorLog(TCColorLogLevelError, HSV_PREFIX, HSV_SUBPREFIX,
				"Can't open 3D LUT Device : %s\n", lut_3d_drv_name[disp]);
			ret = TC_COLOR_ERROR_IO;
		}
		else {
			on_off.lut_3d_onoff = onoff;
			ret = ioctl(fd, TCC_LUT_3D_ONOFF, &on_off);
			if (ret < 0) {
				ret = ColorUtilGetErrorbyErrno((mCLs32)errno);
				TCColorLog(TCColorLogLevelError, HSV_PREFIX, HSV_SUBPREFIX,
					"Can't set 3D LUT on/off : %u, %s\n", on_off.lut_3d_onoff, ColorUtilGetErrorString(ret));
			}
			(void)close(fd);
		}
	}
	return ret;
}

