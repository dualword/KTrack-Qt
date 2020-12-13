####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = ktrack

## INCLUDES were found outside kdevelop specific part

ktrack_SOURCES = mapwidget.cpp transponderdefinitiondetailswidget.ui transponderdefinitiondetailsdialog.cpp transponderdefinitiondialog.cpp transponderdefinitionwidget.ui rigctrl.cpp hamlibwrapper.cpp squintcalculations.cpp squintcalculationsbase.ui satellite.cpp predictionlist.cpp predictionlistbase.ui satellitelistviewitem.cpp groundstationsettings.cpp groundstationsettingsbase.ui calculator.cpp trxwidget.cpp trxwidgetbase.ui globals.cpp transponderdefinitionbase.ui satelliteselection.cpp rigconfig.cpp rigConfigBase.ui satelliteSelectionBase.ui satellitelistview.cpp ktrack.cpp main.cpp dcopinterface.skel
ktrack_LDADD   = ./sgp4sdp4/libsgp4sdp4.a  $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = sgp4sdp4 

EXTRA_DIST = main.cpp ktrack.cpp ktrack.h ktrack.desktop lo32-app-ktrack.png lo16-app-ktrack.png satellitelistview.cpp satellitelistview.h satelliteSelectionBase.ui rigConfigBase.ui rigconfig.cpp rigconfig.h satelliteselection.cpp satelliteselection.h transponderdefinitionbase.ui globals.h globals.cpp trxwidgetbase.ui trxwidget.cpp trxwidget.h calculator.cpp calculator.h groundstationsettingsbase.ui groundstationsettings.cpp groundstationsettings.h satellitelistviewitem.cpp satellitelistviewitem.h hi16-app-ktrack.png hi32-app-ktrack.png predictionlistbase.ui predictionlist.cpp predictionlist.h satellite.cpp satellite.h dcopinterface.h squintcalculationsbase.ui squintcalculations.cpp squintcalculations.h hamlibwrapper.cpp hamlibwrapper.h rigctrl.cpp rigctrl.h transponderdefinitionwidget.ui transponderdefinitiondialog.cpp transponderdefinitiondialog.h transponderdefinitiondetailsdialog.cpp transponderdefinitiondetailsdialog.h transponderdefinitiondetailswidget.ui mapwidget.cpp mapwidget.h 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/HamRadio/
	$(INSTALL_DATA) $(srcdir)/ktrack.desktop $(kde_appsdir)/Applications/HamRadio/ktrack.desktop
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-ktrack.png $(kde_icondir)/locolor/32x32/apps/ktrack.png
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-ktrack.png $(kde_icondir)/locolor/16x16/apps/ktrack.png
	$(mkinstalldirs) $(kde_icondir)/hicolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/hi16-app-ktrack.png $(kde_icondir)/hicolor/16x16/apps/ktrack.png
	$(mkinstalldirs) $(kde_icondir)/hicolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/hi32-app-ktrack.png $(kde_icondir)/hicolor/32x32/apps/ktrack.png

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/HamRadio/ktrack.desktop
	-rm -f $(kde_icondir)/locolor/32x32/apps/ktrack.png
	-rm -f $(kde_icondir)/locolor/16x16/apps/ktrack.png
	-rm -f $(kde_icondir)/hicolor/16x16/apps/ktrack.png
	-rm -f $(kde_icondir)/hicolor/32x32/apps/ktrack.png

####### kdevelop will overwrite this part!!! (end)############
# These paths are KDE specific. Use them:
# kde_appsdir         Where your application's menu entry (.desktop) should go to.
# kde_icondir         Where your icon should go to - better use KDE_ICON.
# kde_sounddir        Where your sounds should go to.
# kde_htmldir         Where your docs should go to. (contains lang subdirs)
# kde_datadir         Where you install application data. (Use a subdir)
# kde_locale          Where translation files should go to. (contains lang subdirs)
# kde_cgidir          Where cgi-bin executables should go to.
# kde_confdir         Where config files should go to (system-wide ones with default values).
# kde_mimedir         Where mimetypes .desktop files should go to.
# kde_servicesdir     Where services .desktop files should go to.
# kde_servicetypesdir Where servicetypes .desktop files should go to.
# kde_toolbardir      Where general toolbar icons should go to (deprecated, use KDE_ICON).
# kde_wallpaperdir    Where general wallpapers should go to.
# kde_templatesdir    Where templates for the "New" menu (Konqueror/KDesktop) should go to.
# kde_bindir          Where executables should go to. Use bin_PROGRAMS or bin_SCRIPTS.
# kde_libdir          Where shared libraries should go to. Use lib_LTLIBRARIES.
# kde_moduledir       Where modules (e.g. parts) should go to. Use kde_module_LTLIBRARIES.
# kde_styledir        Where Qt/KDE widget styles should go to (new in KDE 3).
# kde_designerdir     Where Qt Designer plugins should go to (new in KDE 3).

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
ktrack_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# Uncomment the following two lines if you add a ui.rc file for your application to make use of
# KDE?s XML GUI builing
#rcdir = $(kde_datadir)/ktrack
#rc_DATA = ktrackui.rc

#WARNING: if you use a ui.rc file above, use:

# messages: rc.cpp

# instead of 

# messages:

messages: 
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/ktrack.pot; \
	fi
