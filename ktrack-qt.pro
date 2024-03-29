lessThan(QT_VERSION, 5.0) {
	error('Qt 5.0+ required...')
}

DEFINES *= USE_HAMLIB #hamlib-1.2.3

TEMPLATE = app
TARGET = ktrack-qt
message(Building: = $${TARGET})

CONFIG += thread debug
QT += widgets

DEPENDPATH += . ktrack ktrack/sgp4sdp4
INCLUDEPATH += . ktrack/sgp4sdp4 ktrack

QMAKE_CXXFLAGS += -std=c++11 -fpermissive
LIBS += 

HEADERS += ktrack/calculator.h \
           #ktrack/dcopinterface.h \
           ktrack/globals.h \
           ktrack/satellitelistview.h \
           #ktrack/satellitelistviewitem.h \
           ktrack/groundstationsettings.h \
           ktrack/rigconfig.h \
           ktrack/rigctrl.h \
           ktrack/ktrack.h \
           ktrack/mapwidget.h \
           ktrack/predictionlist.h \
           ktrack/satellite.h \
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
           ktrack/satellitelistview.cpp \
           #ktrack/satellitelistviewitem.cpp \
           ktrack/groundstationsettings.cpp \
           ktrack/rigconfig.cpp \
           ktrack/rigctrl.cpp \
           #ktrack/ktrack_meta_unload.cpp \
           #ktrack/ktrack_skel.cpp \
           ktrack/mapwidget.cpp \
           ktrack/predictionlist.cpp \
           ktrack/satellite.cpp \
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
         ktrack/rigConfigBase.ui \
         ktrack/transponderdefinitiondetailswidget.ui \
         ktrack/transponderdefinitionwidget.ui \
         ktrack/trxwidgetbase.ui
           
           
contains(DEFINES,USE_HAMLIB){
	message(Using: hamlib)
	HEADERS += \
           ktrack/hamlibwrapper.h
           
    SOURCES += \
           ktrack/hamlibwrapper.cpp         
         
	LIBS += -lhamlib

}

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui
