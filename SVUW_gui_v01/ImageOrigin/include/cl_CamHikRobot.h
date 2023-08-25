#ifndef CL_CAMHIKROBOT_H
#define CL_CAMHIKROBOT_H

#include <QObject>

// Qt
//#include <QMainWindow>
//#include <QGraphicsPixmapItem>

// std

#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <string.h>
//#include "windows.h"

// OpenCV
#include <opencv2/opencv.hpp>
//#include "opencv2/core/core_c.h"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"


// HicRobot
#include "MvCameraControl.h"

using namespace std;



class cl_CamHikRobot : public QObject
{
    Q_OBJECT
        public:
                 explicit cl_CamHikRobot(QObject *parent = nullptr);

            // ==============================
            //
            // ==============================
            int HRCam_GetCamList();                             // Получить список подключённых устройств (камер)

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


//            // ==============================
//            // Работа с несколькими камерами по vector<unsigned int> CamIndex_S
//            // ==============================
//            vector<int> HRCam_ConnectToCam_S(vector<unsigned int> CamIndex_S);     // Подключение к устройству (камере)
//            vector<int> HRCam_SetCamSettings_S(vector<unsigned int> CamIndex_S);   // Настройка устройства (камеры)
//            vector<int> HRCam_StartGrabbing_S(vector<unsigned int> CamIndex_S);    // Включение работы устройсва (камеры) - запуск захват кадров

//            vector<cv::Mat> HRCam_GetImage_S(vector<unsigned int> CamIndex_S);     // Получить изображение c устройсва (камеры), полученных во время захвата

//            vector<int> HRCam_StopGrabbing_S(vector<unsigned int> CamIndex_S);    // Остановка работы устройства (камеры)
//            vector<int> HRCam_DisconnecCam_S(vector<unsigned int> CamIndex_S);    // Отключение от усройства


            // ==============================
            //
            // ==============================
            void HRCam_DestroyHandle(unsigned int CamIndex);  // Очистка

            MV_CC_DEVICE_INFO_LIST HRCam_getDeviceList();



        private:
            bool PrintDeviceInfo(MV_CC_DEVICE_INFO *);  // Вывод информации об подключённых камерах



signals:

};

#endif // CL_CAMHIKROBOT_H
