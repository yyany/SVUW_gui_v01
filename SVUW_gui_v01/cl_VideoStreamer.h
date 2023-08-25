#ifndef CL_VIDEOSTREAMER_H
#define CL_VIDEOSTREAMER_H






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







// for convenience
using MJPEGStreamer = nadjieb::MJPEGStreamer;



class cl_VideoStreamer // : public QObject
{

    // Q_OBJECT
public:
   // explicit cl_VideoStreamer(QObject *parent = nullptr);
    cl_VideoStreamer();


    void Start_VideoStream_CycleIn(); // Старт стрима с внутренним циклом


    void StartStreamer(); // Старт стрима
    void VideoStream();   // Цикл стрима
    void Stop_VideoStream(); // Прекращение стрима

    void set_imgStream(cv::Mat);
    cv::Mat get_imgStream();

    void setStreamQualiy(int);
    void setSreamPort (int);
    void setStreamPublishPath (std::string );


public:

   // ADS::cl_DenseStereo *G_obj_ptr; // указатель оъект класса



    std::vector<int> G_params = {cv::IMWRITE_JPEG_QUALITY, 50};
    int G_StreamQualiy = 50;
    int G_SreamPort = 8080;
    std::string G_StreamPublishPath = "/result";

    MJPEGStreamer G_streamer;

   // std::vector<uchar> G_buff_bgr;


    bool G_ExitCode;          // Код для выхода из стрима



    cv::Mat G_frameStream;

    // QElapsedTimer G_timer; // таймер времени для расчёта скорости

    int G_FrameCounter = 0;   // Количество кадров
    int G_StreamSpeed = 0;    // Количество кадров в сек
    int G_StreamSpeed_out = 0;    // Количество кадров в сек


    // =====================================
    // Mutex для потоков
    // =====================================

    std::mutex  G_imgStream_mutex;            //


//signals:



};

#endif // CL_VIDEOSTREAMER_H
