

##win64-msvc*{

# добавление новых модулей
QT       += core gui
QT       += core network # для работы с сетевыми калассами к уже существующему модулю core я добавил модуль network
QT       += serialport    # для предачи данных на Arduino через COM port
QT       += testlib       # для подключения #include <QTest>
QT       += multimedia    # для камер
QT       += datavisualization # для визуализации (Альчаков)
QT       += widgets


# Qt Test - это платформа для модульного тестирования приложений и библиотек на основе Qt.

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# Чтоб дебажить в релизе
CONFIG += force_debug_info
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AnimatedSplitter.cpp \
    ImageOrigin/include/cl_CamBasler.cpp \
    ImageOrigin/include/vcl_ImgResource.cpp \
    QTFormApp_3DRuler/camerascene.cpp \
    QTFormApp_3DRuler/toolwindow.cpp \
    cl_LOG.cpp \
    cl_TCP_client.cpp \
    cl_TCP_server.cpp \
    cl_VideoStreamer.cpp \
    f02_ruler.cpp \
    form01_mainSettings.cpp \
    main.cpp \
    mainwindow.cpp \
    cl_DenseStereo.cpp \
    cl_ImGrabBaslerCam.cpp \
    func_DenseStereo.cpp \
    func_main.cpp

HEADERS += \
    AnimatedSplitter.h \
    ImageOrigin/include/cl_CamBasler.h \
    ImageOrigin/include/vcl_ImgResource.h \
    QTFormApp_3DRuler/camerascene.h \
    QTFormApp_3DRuler/datastructure.h \
    QTFormApp_3DRuler/toolwindow.h \
    QTFormApp_3DRuler/ui_toolwindow.h \
    cl_LOG.h \
    cl_TCP_client.h \
    cl_TCP_server.h \
    cl_VideoStreamer.h \
    f02_ruler.h \
    form01_mainSettings.h \
    h_DenceStereoStructs.h \
    h_main_includes.h \
    mainwindow.h \
    SVA_dll_global.h \
    cl_DenseStereo.h \
    cl_ImGrabBaslerCam.h \
    func_DenseStereo.h \
    func_main.h



FORMS += \
    QTFormApp_3DRuler/toolwindow.ui \
    f02_ruler.ui \
    form01_mainSettings.ui \
    mainwindow.ui






# =================
# INCLUDES
#==================

# OpenCV

## opencv-4.2.0-vc14_vc15
#INCLUDEPATH += \
#                C:\libs\opencv\openCV_MSVC\build\include \


# opencv-4.8.0-vc14_vc15
INCLUDEPATH += \
                C:\libs\opencv\opencv-4.8.0-vc14_vc15\build\include \

#==================

# Armadillo
INCLUDEPATH += \
               C:\libs\armadillo\armadillo-9.850.1\include

#==================

# Pylon
INCLUDEPATH += \
#               $$(PYLON_DEV_DIR)\include \
#               $$(PYLON_DEV_DIR)\include\pylon \
                C:\libs\Pylon\Development\include \
                C:\libs\Pylon\Development\include\pylon \
#==================

# pyclustering

INCLUDEPATH += \
               C:\libs\pyclustering\pyclustering-master\ccore\include


# =================
# LIBS
#==================

# Armadillo
LIBS += -LC:\libs\armadillo\armadillo-9.850.1\examples\lib_win64 \
                 -llapack_win64_MT \
                 -lblas_win64_MT

#LIBS += -LC:\libs\armadillo\armadillo-11.1.1\examples\lib_win64 \
#                   -llibopenblas


# Pylon

# LIBS += -L$$(PYLON_DEV_DIR)/lib/x64 \
LIBS += -LC:\libs\Pylon\Development\lib\x64 \
                -lGCBase_MD_VC141_v3_1_Basler_pylon \
                -lGenApi_MD_VC141_v3_1_Basler_pylon \
                -lPylonBase_v7_1 \
                -lPylonC \
                -lPylonC_v7_1 \
                -lPylonDataProcessing_v1_1 \
                -lPylonGUI_v7_1 \
                -lPylonUtility_v7_1



# pyclustering

#LIBS += -LC:\libs\pyclustering\pyclustering-master\ccore\src \
#                  -lpyclustering
LIBS += -LC:\libs\pyclustering\pyclustering-master\pyclustering\core\64-bit\win  \
                  -lccore
#                  -lpyclustering-static-d



# OpenCV

## opencv-4.2.0-vc14_vc15
#LIBS += -LC:\libs\opencv\openCV_MSVC\build\x64\vc15\lib \
#                 -lopencv_world420
##                 -lopencv_world420d

# opencv-4.8.0-vc14_vc15
LIBS += -LC:\libs\opencv\opencv-4.8.0-vc14_vc15\build\x64\vc16\lib \
                -lopencv_world480
#                 -lopencv_world480d







# HikRobotCam
INCLUDEPATH += \
               C:\libs\HikRobotCam\Includes
DEPENDPATH += \
               C:\libs\HikRobotCam\Includes

LIBS += -LC:\libs\HikRobotCam\Libraries\win64 \
                 -lMvCameraControl




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


##}



##unix-g++{

#QT       += core gui
#QT       += core network # для работы с сетевыми калассами к уже существующему модулю core я добавил модуль network
#QT       +=serialport    # для предачи данных на Arduino через COM port
#QT       +=testlib       # для подключения #include <QTest>
##greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QT += widgets # new
#CONFIG += c++11

## You can make your code fail to compile if it uses deprecated APIs.
## In order to do so, uncomment the following line.
##DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0




## Чтоб дебажить в релизе
#CONFIG += force_debug_info
#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO


#SOURCES += \
#    AnimatedSplitter.cpp \
#    ImageOrigin/include/cl_CamBasler.cpp \
#    ImageOrigin/include/vcl_ImgResource.cpp \
#    cl_LOG.cpp \
#    cl_TCP_client.cpp \
#    cl_TCP_server.cpp \
#    form01_mainSettings.cpp \
#    main.cpp \
#    mainwindow.cpp \
#    cl_DenseStereo.cpp \
#    cl_ImGrabBaslerCam.cpp \
#    func_DenseStereo.cpp \
#    func_main.cpp

#HEADERS += \
#    AnimatedSplitter.h \
#    ImageOrigin/include/cl_CamBasler.h \
#    ImageOrigin/include/vcl_ImgResource.h \
#    cl_LOG.h \
#    cl_TCP_client.h \
#    cl_TCP_server.h \
#    form01_mainSettings.h \
#    mainwindow.h \
#    SVA_dll_global.h \
#    cl_DenseStereo.h \
#    cl_ImGrabBaslerCam.h \
#    func_DenseStereo.h \
#    func_main.h



#FORMS += \
#    form01_mainSettings.ui \
#    mainwindow.ui

#TEMPLATE = app
##TARGET = camera_ui
#PYLON_ROOT = /opt/pylon

## Input
#QMAKE_CPPFLAGS += /opt/pylon/include
#QMAKE_CXXFLAGS += -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-variable
#QMAKE_LFLAGS_RPATH += -Wl,--enable-new-dtags -Wl,-rpath,/opt/pylon/lib

## pylon
#INCLUDEPATH += /opt/pylon/include
##INCLUDEPATH += /opt/pylon/lib
## opencv
#INCLUDEPATH += /usr/local/include/opencv4
## INCLUDEPATH += /usr/local/include/opencv2
## pyclustering
#INCLUDEPATH += /home/otsek/pyclustering-0.10.1.2/ccore/include

##LIBS += 'pkg-config opencv4 --cflags --libs'

#CONFIG += link_pkgconfig
#PKGCONFIG += opencv4

#LIBS += -L/opt/pylon/lib
#LIBS += -lpylonbase -lpylonutility -lGenApi_gcc_v3_1_Basler_pylon -lGCBase_gcc_v3_1_Basler_pylon
#LIBS += -lopencv_core -lopencv_highgui
#LIBS += -larmadillo
#LIBS += -L/home/otsek/pyclustering-0.10.1.2/pyclustering/core/64-bit/linux
#LIBS += -lpyclustering



###HikRobot
#INCLUDEPATH += /opt/MVS/include
#DEPENDPATH += /opt/MVS/include
#LIBS += -L/opt/MVS/bin -L/opt/MVS/lib/64 -lMvCameraControl
##INCLUDEPATH += ${MVCAM_SDK_PATH}/include
##LIBS += -L${MVCAM_SDK_PATH}/bin -L${MVCAM_COMMON_RUNENV}/64 -lMvCameraControl



## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

##}

















## OLD

##}

##unix {

##TEMPLATE = app
##TARGET = camera_ui
#PYLON_ROOT = /opt/pylon

## Input
#QMAKE_CPPFLAGS += /opt/pylon/include
#QMAKE_CXXFLAGS += -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-variable
#QMAKE_LFLAGS_RPATH += -Wl,--enable-new-dtags -Wl,-rpath,/opt/pylon/lib

## pylon
#INCLUDEPATH += /opt/pylon/include
##INCLUDEPATH += /opt/pylon/lib
## opencv
#INCLUDEPATH += /usr/local/include/opencv4
## INCLUDEPATH += /usr/local/include/opencv2
## pyclustering
#INCLUDEPATH += /home/oleg/pyclustering/ccore/include/

##LIBS += 'pkg-config opencv4 --cflags --libs'

#CONFIG += link_pkgconfig
#PKGCONFIG += opencv4

#LIBS += -L/opt/pylon/lib
#LIBS += -lpylonbase -lpylonutility -lGenApi_gcc_v3_1_Basler_pylon -lGCBase_gcc_v3_1_Basler_pylon
#LIBS += -lopencv_core -lopencv_highgui
#LIBS += -larmadillo
#LIBS += -L/home/oleg/pyclustering/pyclustering/core/64-bit/linux/
#LIBS +=  -lpyclustering





## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target


##}

RESOURCES += \
    res_ImgLogo.qrc \
    res_QTFormApp_3DRuler.qrc

