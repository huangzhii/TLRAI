#-------------------------------------------------
#
# Project created by QtCreator 2016-11-30T22:29:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TLRAI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    convnets.cpp \
    detect.cpp

HEADERS  += mainwindow.h \
    convnets.h \
    detect.h

FORMS    += mainwindow.ui
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
#LIBS += -L/usr/local/lib -L/usr/local/cuda/lib64 -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lcufft -lnpps -lnppi -lnppc -lcudart -lrt -lpthread -lm -ldl
LIBS += -L/usr/local/lib -L/usr/local/cuda/lib64 -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lrt -lpthread -lm -ldl


#if: "git push origin master" fail, then do "git push origin master -f". Note that -f is short for --force
