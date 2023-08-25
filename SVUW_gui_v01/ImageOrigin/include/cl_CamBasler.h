#ifndef CL_CAMBASLER_H
#define CL_CAMBASLER_H

#include <QObject>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

// thread
#include <thread>
#include <mutex>

//#include <time.h>
//#include <armadillo>



#if defined(_WIN64)


// ================
// Only Windows
// ================


// ================
// Only Windows END
// ================

#else

// ================
// Only Linux
// ================

#include <unistd.h>

// ================
// Only Linux END
// ================

#endif

#include <func_DenseStereo.h>

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


// OpenCV
#include <opencv2/opencv.hpp>



// Namespace for using pylon objects.
using namespace Pylon;
using namespace Basler_UniversalCameraParams;
// Namespace for using cout.
using namespace std;



class cl_CamBasler : public QObject
{
    Q_OBJECT
        public:
                 explicit cl_CamBasler(QObject *parent = nullptr);



                 void InitializeBaslerCam();
                 bool openBaslerCam();
                 bool closeBaslerCam();

                 void captureContinuous(bool exitCode);





             public:

                 // Тестовое изображение в случаи отсутствия связи с камерами
                 cv::Mat sample;


                 // Инициализация видео записи
                 cv::VideoWriter video_01;
                 cv::VideoWriter video_02;



                 // =====================================
                 // Mutex для потоков
                 // =====================================
                 std::mutex  img_mutex;



                 //    cv::VideoWriter video_01 = cv::VideoWriter("./file/VideoWrite_left_01.mp4", 0, 0, {1200,1920},1);
                 //    cv::VideoWriter video_02 = cv::VideoWriter("./file/VideoWrite_right_02.mp4", 0, 0, {1200,1920},1);


                 // Свойства класса


                 // =============================
                 // Параметры для обрезки изображений
                 // =============================
                 // Параметры настройки камер


                 vector<int>  cam_X = {1920, 1920};     // Ширина активной области (активной зоны) камеры в пикселях
                 vector<int>  cam_Y = {1200, 1200};     // Высота активной области (активной зоны) камеры в пикселях.
                 vector<int> cam_CX = {1, 1};           // Центрирование изображения по горизонтали
                 vector<int> cam_CY = {1, 1};          // Центрирование изображения по вертикали
                 vector<int>  cam_dX = {8, 8};          // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
                 vector<int>  cam_dY = {8, 8};          // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)

                 //acA1300-60gc
                 //     vector<int>  cam_X = {1280, 1280};
                 //     vector<int>  cam_Y = {1024, 1024};


                 // Параметры сжатия изображения
                 //          int  img_X = 960;
                 //          int  img_Y = 600;

                 //    int  img_X = 1440;
                 //    int  img_Y = 900;

                 int  img_X = 960;     // Ширина сжатого изображения
                 int  img_Y = 600;     // Высота сжатого изображения


                 //     //acA1300-60gc
                 //     int  img_X = 1280;
                 //     int  img_Y = 1024;

                 double  img_coeff = 1.0; // Коэфициент восколько раз уменьшено изображение (не связан с размерами img_X img_Y)

                 // =============================
                 // Переменные о состоянии для передачи по TCP
                 // =============================

                 // информация о состоянии StatusStateInformation
                 int StatusSuccessfulOperation_Cam  = 0;    // информация о состоянии

                 // информация о начальном состоянии (только в начале работы) InitialStateInformation
                 int SecondaryPowerSupply_Cam = 1;               // Наличие вторичного электропитания отсека СТЗ
                 int ConnectToCams_Cam = 0;                      // Наличие соединения с камерами
                 int ImageAquisition_Cam = 0;                    // Получение изображения камерами
                 int ReadyForOperationCompartmentLamp_Cam = 1;   // Готовность к работе регулятора яркости светильника отсека
                 int FuctSuccessfulOperation_Cam = 0;            // факт успешной работы




             private:

                 double m_framerate;

                 bool m_canceled = false;

                 double framerate() const;
                 int interpacketDelay() const;

                 //    CTlFactory& tlFactory;
                 //Basler_UniversalCameraParams::UserSetSelectorEnums m_cameraPreset;
                 Pylon::IGigETransportLayer* m_transportLayer;

                 Pylon::DeviceInfoList_t usableDeviceInfos;

                 const String_t subnet;


                 //CBaslerUniversalInstantCamera cameras;
                 Pylon::CBaslerUniversalInstantCameraArray cameras;

                 // =============================
                 // Переменные для Basler камер
                 // =============================
                 CPylonImage pylonImage_0, pylonImage_1;
                 cv::Mat ImgOpenCV_1left, ImgOpenCV_2right;
                 vector<cv::Mat> ImgOpenCV_StereoPair;
                 cv::Mat ImgOpenCV_3leftHand, ImgOpenCV_4rightHand;


                 // Переменные для чтения из видео файла
                 cv::VideoCapture cap_left;
                 cv::VideoCapture cap_right;


                 // uint32_t c_countOfImagesToGrab = 2;
                 size_t c_maxCamerasToUse = 2;   // Количесво задействованных камер

                 const String_t SerialNumber_StereoCam00left =  "22960744";     // Серийный номер левой камеры стереокамеры
                 const String_t SerialNumber_StereoCam01right = "22960745";     // Серийный номер правой камеры стереокамеры
                 const String_t SerialNumber_HandCam02left =    "22960746";     // Серийный номер  камеры на левой руке манипулятора
                 const String_t SerialNumber_HandCam03right =   "22984583";     // Серийный номер  камеры на првавой руке манипулятора


                 // Создание вектора для выбора камер по серийным номерам
                 vector<String_t> SerialNumber_Cams = {SerialNumber_StereoCam00left,
                                                       SerialNumber_StereoCam01right,
                                                       SerialNumber_HandCam02left,
                                                       SerialNumber_HandCam03right};


                 const uint32_t DeviceKey = 0x29;
                 const uint32_t GroupKey  = 0x112233;
                 //        const uint32_t GroupMask = 0xffffffff;





                 // =============================
                 // Переременные для работы IP с камерами симулятора CaptureFromIP()
                 // =============================

                 cv::VideoCapture vcap01, vcap02, vcap03, vcap04;
                 // адреса камер симулятора
                 std::string videoStreamAddressLeft =      "http://127.0.0.1:800";
                 std::string videoStreamAddressRight =     "http://127.0.0.1:801";
                 std::string videoStreamAddressLeftHand =  "http://127.0.0.1:802";
                 std::string videoStreamAddressRightHand = "http://127.0.0.1:803";


                 // Создание вектора для выбора камер по серийным номерам
                 vector<string> StreamAddress_IPcams = {videoStreamAddressLeft,
                                                        videoStreamAddressRight,
                                                        videoStreamAddressLeftHand,
                                                        videoStreamAddressRightHand};










signals:

};

#endif // CL_CAMBASLER_H
