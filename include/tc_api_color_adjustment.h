/****************************************************************************************
 *   FileName    : tc_api_color_adjustment.h
 *   Description : COLOR Adjustment API Header file.
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

#ifndef TC_API_COLOR_ADJUSTMENT_H
#define TC_API_COLOR_ADJUSTMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define BIT_DEPTH (8U)	/* To support 24-bit 256 entries */
#define COLOR_MASK	(0xFFU)

/* LUT Component - disp_num */
#define DISP_DEV0	(0U)	/* using 3D LUT, 1D LUT */
#define DISP_DEV1	(1U)	/* using 3D LUT, 1D LUT */
#define DISP_DEV2	(2U)	/* using 1D LUT */
#define DISP_COMP0	(3U)	/* using 1D LUT */
#define DISP_COMP1	(4U)	/* using 1D LUT */
#define DISP_DEV3	(7U)	/* using 1D LUT */

/* If disp_num select DISP_COMP0, DISP_COMP1,
	plugin_num must be set as below */
#define PLUGIN_RDMA00	(0U)
#define PLUGIN_RDMA01	(1U)
#define PLUGIN_RDMA02	(2U)
#define PLUGIN_RDMA03	(3U)
#define PLUGIN_RDMA04	(4U)
#define PLUGIN_RDMA05	(5U)
#define PLUGIN_RDMA06	(6U)
#define PLUGIN_RDMA07	(7U)
#define PLUGIN_RDMA08	(8U)
#define PLUGIN_RDMA09	(9U)
#define PLUGIN_RDMA10	(10U)
#define PLUGIN_RDMA11	(11U)
#define PLUGIN_RDMA12	(12U)
#define PLUGIN_RDMA13	(13U)
#define PLUGIN_RDMA14	(14U)
#define PLUGIN_RDMA15	(15U)
#define PLUGIN_VIN00	(16U)
#define PLUGIN_RDMA16	(17U)
#define PLUGIN_VIN01	(18U)
#define PLUGIN_RDMA17	(19U)
#define PLUGIN_WDMA00	(20U)
#define PLUGIN_WDMA01	(21U)
#define PLUGIN_WDMA02	(22U)
#define PLUGIN_WDMA03	(23U)
#define PLUGIN_WDMA04	(24U)
#define PLUGIN_WDMA05	(25U)
#define PLUGIN_WDMA06	(26U)
#define PLUGIN_WDMA07	(27U)
#define PLUGIN_WDMA08	(28U)

/* types */
typedef 		 char	mCLChar;
typedef 		 int	mCLs32;
typedef unsigned int	mCLu32;
typedef 		 long	mCLLong;
typedef 		 double mCLDouble;

/*
 * ===	FUNCTION  ======================================================================
 *			   Name :  tc_api_color_initialize
 *		Description :  initialize color api
 *		  Parameter :  none
 *			Returns :
 * =====================================================================================
 */
extern void tc_api_color_initialize(void);

/*
 * ===	FUNCTION  ======================================================================
 *			   Name :  tc_api_set_log_level
 *		Description :  set log level
 *		  Parameter :  log_level (0 ~ 4)
 *			Returns :
 * =====================================================================================
 */
extern void tc_api_set_log_level(mCLu32 log_level);

/*
 * ===	FUNCTION  ======================================================================
 *			   Name :  tc_api_color_set_hue_saturation
 *		Description :  set color hue saturation using 3d lut
 *		  Parameter :  hue_angle : -128 ~ 127, saturation_gain : 0 ~ 1024, disp_num : 0 ~ 1
 *			Returns :  '0': success, 'negative': failed
 *		   Error No :
 * =====================================================================================
 */
extern mCLs32 tc_api_color_set_hue_saturation(mCLs32 hue_angle, mCLs32 saturation_gain, mCLu32 disp_num);

/*
 * ===	FUNCTION  ======================================================================
 *			   Name :  tc_api_color_on_off_3d_lut
 *		Description :  set 3d lut on/off
 *		  Parameter :  onoff : 0 ~ 1, disp_num : 0 ~ 1
 *			Returns :  '0': success, 'negative': failed
 *		   Error No :
 * =====================================================================================
 */
extern mCLs32 tc_api_color_on_off_3d_lut(mCLu32 onoff, mCLu32 disp_num);

/*
 * ===	FUNCTION  ======================================================================
 *			   Name :  tc_api_color_set_gamma
 *		Description :  set gamma using 1d lut
 *		  Parameter :  gamma rgb : 0300 ~ 2800, rgb ground : 0 ~ 255, disp_num : 0 ~ 4, 7
 *			Returns :  '0': success, 'negative': failed
 *		   Error No :
 * =====================================================================================
 */
extern mCLs32 tc_api_color_set_gamma(mCLu32 gamma_red, mCLu32 gamma_green, mCLu32 gamma_blue,
	mCLu32 r_ground, mCLu32 g_ground, mCLu32 b_ground, mCLu32 disp_num, mCLu32 plugin_num);

/*
 * ===  FUNCTION  ======================================================================
 *             Name :  tc_api_color_set_color_temperature
 *      Description :  set color temperature using 1d lut
 *        Parameter :  kelvin(k) : 1000 ~ 40000, disp_num : 0 ~ 4, 7
 *          Returns :  '0': success, 'negative': failed
 *         Error No :
 * =====================================================================================
 */
extern mCLs32 tc_api_color_set_color_temperature(mCLs32 kelvin, mCLu32 disp_num, mCLu32 plugin_num);

#ifdef __cplusplus
}
#endif

#endif	/* TC_API_COLOR_ADJUSTMENT_H */
