bin_PROGRAMS = rs_stattoolgtk
rs_stattoolgtk_SOURCES = rs_stattoolgtk.cpp dialogs.cpp mainwindow.cpp playerview.cpp \
			 clistview.cpp utilities.cpp rsparser.cpp infodialog.cpp \
			 io.cpp comparedialog.cpp comparelistview.cpp

# set the include path found by configure
INCLUDES= $(all_includes) -I/usr/include/cairo

# the library search path.
rs_stattoolgtk_LDFLAGS = $(all_libraries)
rs_stattoolgtk_DEPENDENCIES = 
rs_stattoolgtk_LDADD = -lcurl -lpthread -lgthread-2.0 -L/usr/X11R6/lib \
	-lgtkmm-2.4 -lgdkmm-2.4 -latkmm-1.6 -lgtk-x11-2.0 -lpangomm-1.4 -lglibmm-2.4 \
	-lsigc-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lpango-1.0 \
	-lcairo -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lfreetype -lfontconfig \
	-lXrender -lX11 -lXext -lpng12 -lz -lm
noinst_HEADERS = mainwindow.h playerview.h common.h clistview.h rsparser.h \
			utilities.h infodialog.h comparedialog.h comparelistview.h
