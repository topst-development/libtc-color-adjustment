CC = @CC@
CPP = @CPP@
AM_CFLAGS = -I$(top_srcdir)/include -L$(top_builddir)/src

noinst_PROGRAMS = UT_Color_Adjustment

UT_Color_Adjustment_SOURCES = \
	ut_coloradjustment_cli.c

UT_Color_Adjustment_CFLAGS = \
	 $(AM_CFLAGS)

UT_Color_Adjustment_LDADD = \
	-ltccoloradjustment

