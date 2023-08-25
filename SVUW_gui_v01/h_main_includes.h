#ifndef H_MAIN_INCLUDES_H
#define H_MAIN_INCLUDES_H




#include <QObject>
#include <QElapsedTimer>


#include "nadjieb/mjpeg_streamer.hpp"


// OpenCV
#include <opencv2/opencv.hpp>

// =============================================

#if defined(_WIN64)


// ================
// Only Windows
// ================
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
//#include <windows.h>  // **** возможно лишнее ****
#include <direct.h>
#include <time.h>
#include <ppl.h>


// ================
// Only Windows END
// ================

#else

// ================
// Only Linux
// ================

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <stdlib.h>

// ================
// Only Linux END
// ================

#endif



// Новые библиотеки для определения габаритов объектов
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <list>


//
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

#include <math.h>
#include <ctime>

#include <chrono>
#include <fstream>


// Для Одометрии (Леша)
#include <iomanip>
//#include "Odometry_Include/StereoParams.h"
//#include "Odometry_Include/StereoImage.h"
//#include "Odometry_Include/PointCloud.h"
//#include "Odometry_Include/Trajectory.h"

// thread
#include <thread>
#include <mutex>

// Armadilo
#include <armadillo>


// Pyclustering
#include <pyclustering/definitions.hpp>
#include <pyclustering/cluster/dbscan.hpp>
#include <pyclustering/cluster/dbscan_data.hpp>
#include <pyclustering/cluster/cluster_data.hpp>


// Qt widgets
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QtGui>    // **** возможно лишнее ****

#include <QObject>

#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QtGlobal>




#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPoint>

#include <QFile>


#include <QElapsedTimer>
// Подключение классов
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>

#include <QImage>
#include <QBuffer>
// Поключение класса для вектора для сокетов
#include <QVector>
#include <QFileInfo>
// QSerialPort
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTest> // Qt Test - это платформа для модульного тестирования приложений и библиотек на основе Qt.




// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCameraArray.h>
#include <pylon/gige/GigETransportLayer.h>
#include <pylon/gige/ActionTriggerConfiguration.h>
#include <pylon/gige/BaslerGigEDeviceInfo.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <pylon/gige/PylonGigEIncludes.h>

#ifdef PYLON_WIN_BUILD
#   include <pylon/PylonGUI.h>
#endif


// HicRobot
#include "MvCameraControl.h"

// Класс обработки изображений
//#include <func_DenseStereo.h>
//#include "cl_ImGrabBaslerCam.h"
//#include "cl_DenseStereo.h"



// =============================================










#endif // H_MAIN_INCLUDES_H
