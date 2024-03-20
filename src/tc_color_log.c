/****************************************************************************************
 *	 FileName	 : tc_color_log.c
 *	 Description : This is API function of color logging.
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
#include <stdarg.h>
#include <pthread.h>

#include "tc_api_color_adjustment.h"
#include "tc_color_log.h"

static pthread_mutex_t g_logMutex;
static pthread_mutex_t* g_logMutexPtr = NULL;
static mCLu32 g_level = TCColorLogLevelInfo;
static mCLs32 g_enable = 0;

static const mCLChar *g_logLevelNames[TotalTCColorLogLevels] = {
	"ERROR",
	"WARN",
	"INFO",
	"DEBUG",
	"TABLE"
};

void TCColorLogInitialize(void)
{
	mCLs32 err;

	err = pthread_mutex_init(&g_logMutex, NULL);
	if (err == 0)
	{
		g_logMutexPtr = &g_logMutex;
		TCColorEnableLog(1);
	}
	else
	{
		perror("pthread_mutex_init failed: ");
		(void)fprintf(stderr, "TCColorLog disabled\n");
		TCColorEnableLog(0);
	}
}

void TCColorEnableLog(mCLs32 enable)
{
	g_enable = enable;
}

void TCColorLogSetLevel(mCLu32 level)
{
	if (level < TotalTCColorLogLevels)
	{
		g_level = level;
	}
	else
	{
		(void)fprintf(stderr, "%s: set log level(%u) failed \n", __func__, level);
	}
}

void TCColorLog(mCLu32 level, const mCLChar *prefix, const mCLChar *sub_prefix, const mCLChar *format, ...)
{
	mCLs32 printLog = (level <= g_level) ? 1 : 0;
	if ((g_enable != 0) && (printLog != 0))
	{
		va_list va;

		(void)pthread_mutex_lock(g_logMutexPtr);

		if ((prefix != NULL) && (sub_prefix != NULL))
		{
			(void)fprintf(stdout, "[%s][%s][%s] ",
					g_logLevelNames[level], prefix, sub_prefix);
		}
		else if (prefix != NULL)
		{
			(void)fprintf(stdout, "[%s][%s] ",
					g_logLevelNames[level], prefix);
		}
		else
		{
			(void)fprintf(stdout, "[%s][NO NAME] ",
					g_logLevelNames[level]);
		}

		va_start(va, format);
		(void)vfprintf(stdout, format, va);
		va_end(va);

		(void)pthread_mutex_unlock(g_logMutexPtr);
	}
}

void TCColorLogTable(mCLu32 level, const mCLChar *title, const mCLu32 *table, mCLu32 length)
{
	mCLs32 printLog = (level <= g_level) ? 1 : 0;
	if ((g_enable != 0) && (printLog != 0) && (table != NULL) && (length != 0U))
	{
		mCLu32 tbl_val;
		mCLu32 i;
		mCLu32 mask;
		mCLu32 r;
		mCLu32 g;
		mCLu32 b;

		(void)pthread_mutex_lock(g_logMutexPtr);

		(void)fprintf(stdout, "[%s] %s : ========== \n", g_logLevelNames[level], title);

		mask = COLOR_MASK;
		for (i = 0U; i < length; i++)
		{
			tbl_val = table[i];

			r = (tbl_val & mask);
			g = (tbl_val >> 8U) & mask;
			b = (tbl_val >> 16U) & mask;

			if ((i % 4U) != 0U)
			{
				(void)fprintf(stdout, "|| 0x%08x | %4u, %4u, %4u || ", tbl_val, r, g, b);

				if (i == (length - 1U))
				{
					(void)fprintf(stdout, "\n");
				}
			}
			else
			{
				(void)fprintf(stdout, "\n|| (%4u) | 0x%08x | %4u, %4u, %4u || ", i, tbl_val, r, g, b);
			}
		}
		(void)fprintf(stdout, "============================== \n");

		(void)pthread_mutex_unlock(g_logMutexPtr);
	}
}

