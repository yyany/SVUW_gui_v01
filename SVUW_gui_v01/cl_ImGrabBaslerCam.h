#ifndef CL_IMGRABBASLERCAM_H
#define CL_IMGRABBASLERCAM_H





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


// HicRobot
#include "MvCameraControl.h"


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

// Namespace for using pylon objects.
using namespace Pylon;
using namespace Basler_UniversalCameraParams;
// Namespace for using cout.
using namespace std;






namespace Basler {






class cl_ImGrabBaslerCam: public Pylon::CBaslerUniversalConfigurationEventHandler
{
public:

    cl_ImGrabBaslerCam();
    // virtual ~cl_ImGrabBaslerCam();


    void InitializeBaslerCam();
    bool openBaslerCam();
    bool closeBaslerCam();
    void capture();
    bool checkConection();
    void captureContinuous(bool exitCode);
    void stopCapture();


    void openIPcam();
    void CaptureFromIP();
    void CaptureFromIP_SingleCam(unsigned int CamIndex, bool flag_AlternateGrabCam, bool flag_setImgOpenCV);
    void CaptureFromIP_SingleCam02(unsigned int CamIndex);

    void CaptureFromFile(const char*, const char* );
    void CaptureFromVideo(string, string, vector<bool>*);
    void WriteResaltVideo(const char*);


    void InitializeStereoVideo(const char*);
    void WriteSourseStereoVideo();


    //void releaseStereoVideo();


    cv::Mat getImgOpenCV_1left();
    cv::Mat getImgOpenCV_2right();
    vector<cv::Mat> getImgOpenCV_StereoPair();
    cv::Mat getImgOpenCV_DoubleStereoImg();


    cv::Mat getImgOpenCV_3leftHand();
    cv::Mat getImgOpenCV_4rightHand();

    void setImgOpenCV_1left(cv::Mat);
    void setImgOpenCV_2right(cv::Mat);
    void setImgOpenCV_StereoPair(cv::Mat, cv::Mat);
    void setImgOpenCV_StereoPair_SingleImg(unsigned int idxImg, cv::Mat image01); // для записи не 2-х, а только 1-ой по выбору
    void setFramerate(double framerate);
    void setInterpacketDelay(int interpacketDelay);
    void setCameraPreset(Basler_UniversalCameraParams::UserSetSelectorEnums cameraPreset);
    void setImgOpenCV_1left(double framerate);

    // Количество используемых камер
    int get_c_maxCamerasToUse();
    void set_c_maxCamerasToUse(int);

    // Серийные номера используемых камер
    vector<String_t> get_SerialNumber_Cams();
    void set_SerialNumber_Cams(vector<String_t>);

    // IP адреса IP камер
    vector<string> get_StreamAddress_IPcams();
    void set_StreamAddress_IPcams(vector<string>);


    // =====================================
    // Настройка размера изображения в Basler камер
    // =====================================
    void set_BaslerСamsCrop (vector<int> cam_X,
                         vector<int> cam_Y,
                         vector<int> cam_CX,
                         vector<int> cam_CY,
                         vector<int> cam_dX,
                         vector<int> cam_dY);

    // Настройка параметров сжатия изображения
    void set_ImgCompress (int  img_X, int  img_Y, double  img_coeff);





    // ==============================
    // HikRobot (HikVision)
    // ==============================
    int HRCam_GetCamList();                             // Получить список устройств (камер)

    unsigned int HRCam_FindCamBySN(std::string CamSN); // Найти устройство (камеру) по серийному номеру. Возращает номер  камеры в списке камер
    int HRCam_CreateHandle(unsigned int CamIndex);     // Создать описание выбранного устройства (камеры)
    int HRCam_CreateHandleAllCam();                    // Создать описание всех устройств (камер)


    // ==============================
    // Работа с оной камерой или с каждой по отдельности
    // ==============================
    int HRCam_ConnectToCam(unsigned int CamIndex);     // Подключение к устройству (камере)
    int HRCam_SetCamSettings(unsigned int CamIndex);   // Настройка устройства (камеры)
    int HRCam_StartGrabbing(unsigned int CamIndex);    // Включение работы устройсва (камеры) - запуск захват кадров


    cv::Mat HRCam_GetImage(unsigned int CamIndex);     // Получить изображение c устройсва (камеры), полученных во время захвата

    int HRCam_StopGrabbing(unsigned int CamIndex);    // Остановка работы устройства (камеры)
    int HRCam_DisconnecCam(unsigned int CamIndex);    // Отключение от усройства


    // ==============================
    //
    // ==============================
    void HRCam_DestroyHandle();  // Очистка

    MV_CC_DEVICE_INFO_LIST HRCam_getDeviceList(); // получение списка устройств (камер)

    bool HRCam_IsDeviceConnected(unsigned int CamIndex); // проверка на подключение устройств (камер)



    // =============================
    // Передача переменных о состоянии для отправки по TCP
    // =============================
//     void GetStatusStateInformation();






private:

    // ==============================
    // HikRobot (HikVision)
    // ==============================

    bool PrintDeviceInfo(MV_CC_DEVICE_INFO *);  // Вывод информации об подключённых камерах






public:


   unsigned int flag_CurrentIPCam = 0; // флаг, который показывает какая камера отзахватывалась


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


     vector<int> cam_X = {1920, 1920};     // Ширина активной области (активной зоны) камеры в пикселях
     vector<int> cam_Y = {1200, 1200};     // Высота активной области (активной зоны) камеры в пикселях.
     vector<int> cam_CX = {1, 1};           // Центрирование изображения по горизонтали
     vector<int> cam_CY = {1, 1};          // Центрирование изображения по вертикали
     vector<int> cam_dX = {8, 8};          // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
     vector<int> cam_dY = {8, 8};          // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)

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
     int ReadyForOperationCompartmentLamp_Cam = 0;   // Готовность к работе регулятора яркости светильника отсека
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




    // ==============================
    // HikRobot (HikVision)
    // ==============================




//    std::vector <void*> handle1__;   // дискриптор устройства
//    MV_CC_DEVICE_INFO_LIST HR_stDeviceList_;         // 1-ый список всех устройств в сети, которые есть в наличии

//    cv::Mat HR_Img_;   // Изображение с камеры

//    int HR_nBufSize__;

//    unsigned char* HR_pFrameBuf__;

//    MV_FRAME_OUT_INFO_EX HR_stInfo_;








};

} //namespace Basler

#endif // CL_IM_GRAB_H
