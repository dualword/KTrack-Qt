####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = ktrack po doc 

EXTRA_DIST = ktrack.kdevprj admin AUTHORS COPYING ChangeLog INSTALL README TODO ktrack.lsm nasa.tle earth.jpg night.jpg 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/ktrack/
	$(INSTALL_DATA) $(srcdir)/nasa.tle $(kde_datadir)/ktrack/nasa.tle
	$(mkinstalldirs) $(kde_datadir)/ktrack/
	$(INSTALL_DATA) $(srcdir)/earth.jpg $(kde_datadir)/ktrack/earth.jpg
	$(mkinstalldirs) $(kde_datadir)/ktrack/
	$(INSTALL_DATA) $(srcdir)/night.jpg $(kde_datadir)/ktrack/night.jpg

uninstall-local:
	-rm -f $(kde_datadir)/ktrack/nasa.tle
	-rm -f $(kde_datadir)/ktrack/earth.jpg
	-rm -f $(kde_datadir)/ktrack/night.jpg

####### kdevelop will overwrite this part!!! (end)############
# not a GNU package. You can remove this line, if 
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 

$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files 

package-messages:
	$(MAKE) -f admin/Makefile.common package-messages
	$(MAKE) -C po merge

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
