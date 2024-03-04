TEMPLATE = app
TARGET = ktrack-qt

CONFIG += qt thread debug
QT += qt3support

DEPENDPATH += . ktrack ktrack/sgp4sdp4
INCLUDEPATH += . ktrack/sgp4sdp4 ktrack

QMAKE_CXXFLAGS += -std=c++11
LIBS += 

HEADERS += ktrack/calculator.h \
           #ktrack/dcopinterface.h \
           ktrack/globals.h \
           ktrack/groundstationsettings.h \
           #ktrack/hamlibwrapper.h \
           #ktrack/rigconfig.h \
           #ktrack/rigctrl.h \
           ktrack/ktrack.h \
           ktrack/mapwidget.h \
           ktrack/predictionlist.h \
           ktrack/satellite.h \
           ktrack/satellitelistview.h \
           ktrack/satellitelistviewitem.h \
           ktrack/satelliteselection.h \
           ktrack/squintcalculations.h \
           ktrack/transponderdefinitiondetailsdialog.h \
           ktrack/transponderdefinitiondialog.h \
           ktrack/trxwidget.h \
           ktrack/sgp4sdp4/sgp4sdp4.h
                    
SOURCES += ktrack/main.cpp \
           ktrack/ktrack.cpp \
           ktrack/calculator.cpp \
           ktrack/globals.cpp \
           ktrack/groundstationsettings.cpp \
           #ktrack/hamlibwrapper.cpp \
           #ktrack/rigconfig.cpp \
           #ktrack/rigctrl.cpp \
           #ktrack/ktrack_meta_unload.cpp \
           #ktrack/ktrack_skel.cpp \
           ktrack/mapwidget.cpp \
           ktrack/predictionlist.cpp \
           ktrack/satellite.cpp \
           ktrack/satellitelistview.cpp \
           ktrack/satellitelistviewitem.cpp \
           ktrack/satelliteselection.cpp \
           ktrack/squintcalculations.cpp \
           ktrack/transponderdefinitiondetailsdialog.cpp \
           ktrack/transponderdefinitiondialog.cpp \
           ktrack/trxwidget.cpp \
           ktrack/sgp4sdp4/sgp4sdp4.c \
           ktrack/sgp4sdp4/sgp_in.c \
           ktrack/sgp4sdp4/sgp_math.c \
           ktrack/sgp4sdp4/sgp_obs.c \
           ktrack/sgp4sdp4/sgp_time.c \
           ktrack/sgp4sdp4/solar.c

FORMS += ktrack/groundstationsettingsbase.ui \
         ktrack/predictionlistbase.ui \
         ktrack/satelliteSelectionBase.ui \
         ktrack/squintcalculationsbase.ui \
         #ktrack/transponderdefinitionbase.ui
         #ktrack/rigConfigBase.ui \
         ktrack/transponderdefinitiondetailswidget.ui \
         ktrack/transponderdefinitionwidget.ui \
         ktrack/trxwidgetbase.ui
           
OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui
