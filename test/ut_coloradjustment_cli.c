/****************************************************************************************
 *	 FileName	 : ut_coloradjustment_cli.c
 *	 Description : This is UnitTest Application using CLI.
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
#include <unistd.h>

#include "tc_api_color_adjustment.h"

static void SelectLogLevelAPIMenu(void)
{
	mCLu32 log_level = 0;

	(void)printf("===========================================================\n");
	(void)printf(" 1. Color Adjustment Set Log Level \n");
	(void)printf("===========================================================\n");
	(void)printf("(0) Error : Error logging\n");
	(void)printf("(1) Warning : Warn logging\n");
	(void)printf("(2) Info : Info logging (default)\n");
	(void)printf("(3) Debug : Debug logging\n");
	(void)printf("(4) Table : Table logging\n ");
	(void)printf("Select log level :");
	(void)scanf("%u", &log_level);

	switch (log_level)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			tc_api_set_log_level(log_level);
			(void)printf("Set log level : %u\n", log_level);
			break;
		default:
			(void)printf("Don't set log level %u return \n", log_level);
			break;
	}
	(void)printf("[Back to Main Menu]\n");
}

static void SelectHueSaturationAPIMenu(void)
{
	mCLu32 nMenu = 0U;
	mCLu32 on_off = 0U;
	mCLs32 angle = 0;
	mCLs32 gain = 0;
	mCLu32 disp = 0U;
	mCLu32 backtoMenu = 0U;
	mCLs32 ret;

	while(1)
	{
		(void)printf("===========================================================\n");
		(void)printf("[2. Hue Saturation(LUT 3D) API] is selected\n");
		(void)printf("===========================================================\n");
		(void)printf("(1) LUT 3D On/Off Test\n");
		(void)printf("(2) Hue Saturation Test\n");
		(void)printf("(0) Back to Main Menu\n ");
		(void)printf("Select Menu:");
		(void)scanf("%d", &nMenu);

		switch(nMenu)
		{
			case 0:
				(void)printf("[Back to Main Menu]\n");
				backtoMenu = 1U;
				break;
			case 1:
				(void)printf("[HS] LUT 3D On/Off(On : 1, Off : 0) : ");
				(void)scanf("%u", &on_off);
				(void)printf("[HS] DISP Number(0,1) : ");
				(void)scanf("%u", &disp);
				(void)printf("[HS] LUT 3D On/Off : %u\n", on_off);
				(void)printf("[HS] DISP Number : %u\n", disp);
				ret = tc_api_color_on_off_3d_lut(on_off, disp);
				if (ret < 0) {
					(void)printf("[HS] Fail. LUT 3D On/Off API Return : %d\n", ret);
				}
				else {
					(void)printf("[HS] Success. LUT 3D On/Off API Return : %d\n", ret);
				}
				break;
			case 2:
				(void)printf("[HS] HUE Angle (-128 ~ 127) : ");
				(void)scanf("%d", &angle);
				(void)printf("[HS] Saturation gain (0 ~ 1024) : ");
				(void)scanf("%d", &gain);
				(void)printf("[HS] DISP Number(0,1)  : ");
				(void)scanf("%u", &disp);
				(void)printf("[HS] Hue Angle : %d\n", angle);
				(void)printf("[HS] Saturation gain : %d\n", gain);
				(void)printf("[HS] DISP Number : %u\n", disp);
				ret = tc_api_color_set_hue_saturation(angle, gain, disp);
				if (ret < 0) {
					(void)printf("[HS] Fail. Set Hue Saturation API Return : %d\n", ret);
				}
				else {
					(void)printf("[HS] Success. Set Hue Saturation API Return : %d\n", ret);
				}
				break;
			default:
				(void)printf("[HS] You chose the wrong number. again!");
				break;
		}

		if (backtoMenu == 1U) {
			return;
		}
	}
}

static void SelectTemperatureGammaAPIMenu(void)
{
	mCLu32 nMenu = 0U;
	mCLu32 on_off = 0U;
	mCLu32 plugin = 0U;
	mCLs32 kevin = 0;
	mCLu32 disp = 0U;
	mCLu32 gamma_red = 0U;
	mCLu32 gamma_green = 0U;
	mCLu32 gamma_blue = 0U;
	mCLu32 ground_red = 0U;
	mCLu32 ground_green = 0U;
	mCLu32 ground_blue = 0U;
	mCLu32 backtoMenu = 0U;
	mCLs32 ret;

	while(1)
	{
		(void)printf("===========================================================\n");
		(void)printf("[2. Temperature(LUT) API] is selected\n");
		(void)printf("===========================================================\n");
		(void)printf("(1) LUT Off Test\n");
		(void)printf("(2) Temperature API Test\n");
		(void)printf("(3) Gamma API Test\n");
		(void)printf("(0) Back to Main Menu\n ");
		(void)printf("Select Menu:");
		(void)scanf("%d", &nMenu);

		switch(nMenu)
		{
			case 0:
				(void)printf("[Back to Main Menu]\n");
				backtoMenu = 1U;
				break;
			case 1:
				(void)printf("[TEMP] LUT Component Number(0 ~ 4, 7) : ");
				(void)scanf("%u", &disp);
				(void)printf("[TEMP] LUT Plugin Number(0 ~ 28) : ");
				(void)scanf("%u", &plugin);
				(void)printf("[TEMP] LUT Off\n");
				(void)printf("[TEMP] LUT Component Number : %u, %u\n", disp, plugin);
				ret = tc_api_color_set_color_temperature(0, disp, plugin);
				if (ret < 0) {
					(void)printf("[TEMP] Fail. LUT 1D Off API Return : %d\n", ret);
				}
				else {
					(void)printf("[TEMP] Success. LUT 1D Off API Return : %d\n", ret);
				}
				break;
			case 2:
				(void)printf("[TEMP] Temperature (1000 ~ 40000) : ");
				(void)scanf("%d", &kevin);
				(void)printf("[TEMP] LUT Component Number(0 ~ 4, 7)  : ");
				(void)scanf("%u", &disp);
				(void)printf("[TEMP] LUT Plugin Number(0 ~ 28) : ");
				(void)scanf("%u", &plugin);
				(void)printf("[TEMP] Temperature : %d\n", kevin);
				(void)printf("[TEMP] LUT Component Number : %u, %u\n", disp, plugin);
				ret = tc_api_color_set_color_temperature(kevin, disp, plugin);
				if (ret < 0) {
					(void)printf("[TEMP] Fail. Set Temperature API Return : %d\n", ret);
				}
				else {
					(void)printf("[TEMP] Success. Set Temperature API Return : %d\n", ret);
				}
				break;
			case 3:
				(void)printf("[GAMMA] Gamma Red (0300 ~ 2800) : ");
				(void)scanf("%u", &gamma_red);
				(void)printf("[GAMMA] Gamma Green (0300 ~ 2800) : ");
				(void)scanf("%u", &gamma_green);
				(void)printf("[GAMMA] Gamma Blue (0300 ~ 2800) : ");
				(void)scanf("%u", &gamma_blue);
				(void)printf("[GAMMA] Ground Red (0 ~ 255) : ");
				(void)scanf("%u", &ground_red);
				(void)printf("[GAMMA] Ground Green (0 ~ 255) : ");
				(void)scanf("%u", &ground_green);
				(void)printf("[GAMMA] Ground Blue (0 ~ 255) : ");
				(void)scanf("%u", &ground_blue);
				(void)printf("[GAMMA] LUT Component Number(0 ~ 4, 7) : ");
				(void)scanf("%u", &disp);
				(void)printf("[TEMP] LUT Plugin Number(0 ~ 28) : ");
				(void)scanf("%u", &plugin);
				(void)printf("[GAMMA] Gamma R,G,B : %u,%u,%u\n", gamma_red, gamma_green, gamma_blue);
				(void)printf("[GAMMA] Ground R,G,B : %u,%u,%u\n", ground_red, ground_green, ground_blue);
				(void)printf("[GAMMA] LUT Component Number : %u, %u\n", disp, plugin);
				ret = tc_api_color_set_gamma(gamma_red, gamma_green, gamma_blue,
					ground_red, ground_green, ground_blue, disp, plugin);
				if (ret < 0) {
					(void)printf("[TEMP] Fail. Set Gamma API Return : %d\n", ret);
				}
				else {
					(void)printf("[TEMP] Success. Set Gamma API Return : %d\n", ret);
				}
				break;
			default:
				(void)printf("[TEMP] You chose the wrong number. again!");
				break;
		}

		if (backtoMenu == 1U) {
			return;
		}
	}
}

static void SelectMainMenu(void)
{
	mCLu32 nMenu = 0U;
	mCLu32 backtoMenu = 0U;

	while (1)
	{
		(void)printf("===========================================================\n");
		(void)printf("This application is made for testing Color Adjustment \n");
		(void)printf("===========================================================\n");
		(void)printf("(1) Set LogLevel\n");
		(void)printf("(2) Hue Saturation(LUT 3D) API Test\n");
		(void)printf("(3) Temperature/Gamma(LUT) API Test\n");
		(void)printf("(0) Exit Application\n ");
		(void)printf("Select Menu:");
		(void)scanf("%u", &nMenu);

		switch (nMenu)
		{
			case 1:
				SelectLogLevelAPIMenu();
				break;
			case 2:
				SelectHueSaturationAPIMenu();
				break;
			case 3:
				SelectTemperatureGammaAPIMenu();
				break;
			case 0:
				backtoMenu = 1U;
				break;
			default:
				(void)printf("[TEMP] You chose the wrong number. again!");
				break;
		}

		if (backtoMenu == 1U) {
			return;
		}
	}
}

mCLs32 main(mCLs32 argc, mCLChar** argv)
{
	(void)argc;
	(void)argv;

	(void)printf("[START] Unit Test for Color Adjustment\n");

	SelectMainMenu();

	(void)printf("[EXIT] Unit Test for Color Adjustment\n");

	return 0;
}
