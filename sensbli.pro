TEMPLATE = app
TARGET = sensbli
QT += core \
    gui \
    xml \
    xmlpatterns \
    network \
    svg
HEADERS += blinksensor/CRecordProcessor.h \
    recording.h \
    calibview.h \
    setup.h \
    config.h \
    blinksensor/CBlinkProcessor.h \
    blinksensor/CBlinkSensorDebugger.h \
    blinksensor/CCalibProcessor.h \
    blinksensor/CFrameProcessor.h \
    bloblib/Blob.h \
    bloblib/BlobExtraction.h \
    bloblib/BlobLibraryConfiguration.h \
    bloblib/BlobResult.h \
    cvlib/cvlib.h \
    eyefinder/CBlobPair.h \
    eyefinder/CEyeFinder.h \
    eyefinder/ovroi.h \
    eyestatehandler/CEyeStateHandler.h \
    eyetracker/CEyeTracker.h \
    imagedb/CImageDB.h \
    QOpenCVWidget.h \
    sensbli.h
SOURCES += blinksensor/CRecordProcessor.cpp \
    recording.cpp \
    calibview.cpp \
    setup.cpp \
    blinksensor/CBlinkProcessor.cpp \
    blinksensor/CBlinkSensorDebugger.cpp \
    blinksensor/CCalibProcessor.cpp \
    blinksensor/CFrameProcessor.cpp \
    bloblib/Blob.cpp \
    bloblib/BlobExtraction.cpp \
    bloblib/BlobResult.cpp \
    cvlib/cvlib.cpp \
    eyefinder/CBlobPair.cpp \
    eyefinder/CEyeFinder.cpp \
    eyestatehandler/CEyeStateHandler.cpp \
    eyetracker/CEyeTracker.cpp \
    imagedb/CImageDB.cpp \
    QOpenCVWidget.cpp \
    main.cpp \
    sensbli.cpp
FORMS += recording.ui \
    calibview.ui \
    setup.ui \
    sensbli.ui
RESOURCES += sensbli.qrc

# INCLUDEPATH += "c:\OpenCV2.0\include\opencv"
# LIBS += -L"c:\OpenCV2.0\lib"
# LIBS += -lcv200 \
# -lcxcore200 \
# -lhighgui200
INCLUDEPATH += "c:\OpenCV11\cv\include"
INCLUDEPATH += "c:\OpenCV11\cvaux\include"
INCLUDEPATH += "c:\OpenCV11\cxcore\include"
INCLUDEPATH += "c:\OpenCV11\otherlibs\highgui"
LIBS += -L"c:\OpenCV11\lib"
LIBS += -lcv \
    -lcxcore \
    -lhighgui
RC_FILE = sensbli.rc
TRANSLATIONS = sensbli_de.ts