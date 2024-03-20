CC = @CC@
CPP = @CPP@
AM_CFLAGS = $(TCCOLORADJUSTMENT_CFLAGS) -I$(top_srcdir)/include
LIBS = @LIBS@ $(TCCOLORADJUSTMENT_LIBS)

lib_LTLIBRARIES = libtccoloradjustment.la

libtccoloradjustment_la_SOURCES = \
	tc_api_color_adjustment.c \
	tc_color_3d_tables.c \
	tc_color_hsv.c \
	tc_color_log.c \
	tc_color_util.c \
	tc_color_temperature.c

libtccoloradjustment_la_LIBADD = -lm
libtccoloradjustment_la_LDFLAGS = -version-info $(TCCOLORADJ_VERSION_INFO)
