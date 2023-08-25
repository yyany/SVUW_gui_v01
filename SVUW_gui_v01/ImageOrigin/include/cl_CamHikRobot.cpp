#include "cl_CamHikRobot.h"

// Qt
#include <QMainWindow>
#include <QGraphicsPixmapItem>

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


#define MAX_DEVICE_NUM          24

std::vector <void*> handle1 (MAX_DEVICE_NUM, NULL);   // дискриптор устройства
MV_CC_DEVICE_INFO_LIST stDeviceList1 = { 0 };         // 1-ый список всех устройств в сети, которые есть в наличии

cv::Mat Img;   // Изображение с камеры


//bool ExitCode_GetImg = true;                        // Код выхода из цикла получения изображений

// MV_CC_DEVICE_INFO_LIST stUseDeviceList2 = { 0 };      // 2-ой список используемых устройств
//std::vector <void*> handle2;                          // дискриптор устройства (2-ой список)




//    #define MAX_BUF_SIZE    (2048*1536*3)
#define MAX_BUF_SIZE    (960*720*3)
int nBufSize = MAX_BUF_SIZE;

//unsigned char*  pFrameBuf = NULL;
//pFrameBuf = (unsigned char*)malloc(nBufSize);

unsigned char*  pFrameBuf = (unsigned char*)malloc(nBufSize);

MV_FRAME_OUT_INFO_EX stInfo;






using namespace std;


cl_CamHikRobot::cl_CamHikRobot(QObject *parent)
    : QObject{parent}
{



}


// Print the discovered devices' information
bool cl_CamHikRobot::PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // print current ip and user defined name
        printf("\t CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("\t UserDefinedName: %s\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
        printf("\t Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
        printf("\t Device chManufacturerName: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerName);
        printf("\t Device chDeviceVersion: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chDeviceVersion);
        printf("\t Device chSerialNumber: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber);
        //  printf("\t Device chManufacturerSpecificInfo: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerSpecificInfo);


        //        std::cout << " UserDefinedName: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName << std::endl;
        //        std::cout << " Device Model Name: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName << std::endl;
        //        std::cout << " Device chManufacturerName: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerName << std::endl;
        //        std::cout << " Device chManufacturerSpecificInfo: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerSpecificInfo << std::endl;
        //        std::cout << " Device chDeviceVersion: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chDeviceVersion << std::endl;
        std::cout << " Serial Number: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber << std::endl;


        stringstream ff;
        ff << pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber;
        std::cout << " Serial Number2: " << ff.str() << std::endl;

    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
        printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}




int cl_CamHikRobot::HRCam_GetCamList()
{

    int nRet = -1;       //



    // eEnum device
    // Перечислим все устройства, соответствующие указанному протоколу передачи в подсети
    unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;

    // Функция memset заполняет первые n байт области памяти, на которую указывает аргумент destination,
    // символом, код которого указывается аргументом c.
    //        Синтаксис:

    //#include < string.h >
    //            void *memset (void *destination, int c, size_t n);

    //        Аргументы:

    //                destination – указатель на заполняемый массив
    //            с – код символа для заполнения
    //            n – размер заполняемой части массива в байтах


    memset(&stDeviceList1, 0, sizeof(MV_CC_DEVICE_INFO_LIST));








    // Enum device
    nRet = MV_CC_EnumDevices(nTLayerType, &stDeviceList1);


    if (nRet != 0)
    {
        printf("Enum Devices fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    if (stDeviceList1.nDeviceNum == 0)
    {
        printf("no camera found!\n");
        return nRet;
    }


    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> 3 Enum device" << std::endl;
    // +++++++++++++++++++++++++++++





    // Show devices
    // Вывод информации об устройсвах в терминал
    if (stDeviceList1.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i < stDeviceList1.nDeviceNum; i++)
        {
            printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList1.pDeviceInfo[i];
            if (NULL == pDeviceInfo)
            {
                break;
            }


            // Печать информации об устройсве
            PrintDeviceInfo(pDeviceInfo);
        }
    }
    else
    {
        qDebug() << ("Find No Devices!\n");
        return -1;
    }




    return nRet;
}

unsigned int cl_CamHikRobot::HRCam_FindCamBySN(std::string CamSN) // CamSN - серийный номер камеры
{

    unsigned int CamIndex = -1; // индекс камеры

    for (unsigned int q = 0; q < (unsigned int)stDeviceList1.nDeviceNum; ++q)
    {


        stringstream ff;
        ff << stDeviceList1.pDeviceInfo[q]->SpecialInfo.stGigEInfo.chSerialNumber;


        //        // +++++++++++++++++++++++++++++
        //        std::cout << " \t >>> ff.str() == CamSN" << (ff.str() == CamSN ) << std::endl;
        //        std::cout << " \t >>> ff.str() = " << ff.str()   << std::endl;
        //        std::cout << " \t >>> CamSN = " << CamSN  << std::endl;
        //        // +++++++++++++++++++++++++++++

        if ( ff.str() == CamSN)
        {
            CamIndex = q;

            // +++++++++++++++++++++++++++++
            std::cout << " \t >>> Device has found by Serial Number :: CamIndex = " << CamIndex << std::endl;
            // +++++++++++++++++++++++++++++

            return CamIndex;

        }


        if (CamIndex < 0)
        {
            // +++++++++++++++++++++++++++++
            std::cout << " \t >>> Device has not found by Serial Number :: " << CamSN << std::endl;
            // +++++++++++++++++++++++++++++

        }


    }


    return CamIndex;

}



int cl_CamHikRobot::HRCam_CreateHandle(unsigned int CamIndex)
// CamIndex - номер устройства
{
    int nRet = -1;         // статус выполнения



    // Create handle
    // создайте дескриптор устройства

    //        MV_CC_DEVICE_INFO stDevInfo = {0};
    //        memcpy(&stDevInfo, stDeviceList.pDeviceInfo[nIndex], sizeof(MV_CC_DEVICE_INFO));
    //        nRet[nIndex] = MV_CC_CreateHandle(&handle[nIndex], &stDevInfo);


    //    void *  memcpy ( void *  dest,  const void *  src,  std:: size_t  count  ) ;
    //    Копирует count байты из объекта, на который указывает , srcв объект, на который указывает dest.
    //        Оба объекта интерпретируются как массивы символов без знака.
    //            Параметры
    //            dest	-	указатель на ячейку памяти для копирования в
    //            Src	-	указатель на ячейку памяти для копирования
    //            количество	-	количество байтов для копирования




    nRet = MV_CC_CreateHandle(&handle1[CamIndex], stDeviceList1.pDeviceInfo[CamIndex]);


    if (MV_OK != nRet)
    {
        cout << "DeviceNum = " << CamIndex << "  ";
        printf("Create Handle fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>>  Create Handle, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++

    return nRet;
}



int cl_CamHikRobot::HRCam_CreateHandleAllCam()
{

    int nRet = -1;         // статус выполнения



    // Create handle
    // создайте дескриптор устройства

    //        MV_CC_DEVICE_INFO stDevInfo = {0};
    //        memcpy(&stDevInfo, stDeviceList.pDeviceInfo[nIndex], sizeof(MV_CC_DEVICE_INFO));
    //        nRet[nIndex] = MV_CC_CreateHandle(&handle[nIndex], &stDevInfo);



    for(size_t q = 0; q < stDeviceList1.nDeviceNum; ++q)
    {





        nRet = MV_CC_CreateHandle(&handle1[q], stDeviceList1.pDeviceInfo[q]);

        //        std::cout << handle2[q]->SpecialInfo.stGigEInfo.chSerialNumber;
        //        std::cout << stDeviceList2.pDeviceInfo[q]->SpecialInfo.stGigEInfo.chSerialNumber;


        if (MV_OK != nRet)
        {
            cout << "DeviceNum = " << q << "  ";
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            return nRet;
        }


        // +++++++++++++++++++++++++++++
        std::cout << " \t >>>  Create Handles for All Cam = " << q <<  std::endl;
        // +++++++++++++++++++++++++++++

    }




    return nRet;

}




int cl_CamHikRobot::HRCam_ConnectToCam(unsigned int CamIndex)
{

    int nRet = -1;         // статус выполнения

    // Open device
    // Подключаем устройство
    unsigned int nAccessMode = MV_ACCESS_Exclusive;
    unsigned short nSwitchoverKey = 0;

    if (CamIndex < 0 || CamIndex > stDeviceList1.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", Input Device Num error!" << endl;
        return nRet;
    }


    nRet = MV_CC_OpenDevice(handle1[CamIndex], nAccessMode, nSwitchoverKey);
    // nRet = MV_CC_OpenDevice(handle1[CamIndex] );


    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("Open Device fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>>  Device is Open, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++

    return nRet;

}

int cl_CamHikRobot::HRCam_SetCamSettings(unsigned int CamIndex)
{

    int nRet = -1;         // статус выполнения


    if (CamIndex < 0 || CamIndex > stDeviceList1.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", Input Device Num error!" << endl;
        return nRet;
    }



    // Detection network optimal package size (It only works for the GigE camera)
    if (stDeviceList1.pDeviceInfo[CamIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(handle1[CamIndex]);


        if (nPacketSize > 0)
        {
            nRet = MV_CC_SetIntValue(handle1[CamIndex],"GevSCPSPacketSize",nPacketSize);

            // +++++++++++++++++++++++++++++
            std::cout << " \t >>>  Set Int Value, CamIndex = " <<  CamIndex << std::endl;
            // +++++++++++++++++++++++++++++

            if(nRet != MV_OK)
            {
                cout << "CamIndex = " << CamIndex << "  ";
                printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
            }
        }
        else
        {
            cout << "CamIndex = " << CamIndex << "  ";
            printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
        }
    }






    // Set trigger mode as off
    nRet = MV_CC_SetEnumValue(handle1[CamIndex], "TriggerMode", 0);


    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> Set Trigger Mode, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++




    //  unsigned char * pData = NULL;


    //        // Get payload size
    //        // Получить размер кадра данных
    //        MVCC_INTVALUE stParam;
    //        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    //        nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
    //        if (MV_OK != nRet)
    //        {
    //            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
    //            break;
    //        }
    //        unsigned int nPayloadSize = stParam.nCurValue;


    //        // int nBufSize = stParam.nCurValue; // Размер данных одного кадра


    //        // +++++++++++++++++++++++++++++
    //        std::cout << " \t >>> 10 Get payload size" << std::endl;
    //        // +++++++++++++++++++++++++++++


    //        // Init image info
    //        MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
    //        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    //        pData = (unsigned char *)malloc(sizeof(unsigned char)* (nPayloadSize));
    //        if (NULL == pData)
    //        {
    //            printf("Allocate memory failed.\n");
    //            break;
    //        }
    //        memset(pData, 0, sizeof(pData));


    //        // +++++++++++++++++++++++++++++
    //        std::cout << " \t >>> 11 Init image info" << std::endl;
    //        // +++++++++++++++++++++++++++++


    return nRet;

}

int cl_CamHikRobot::HRCam_StartGrabbing(unsigned int CamIndex)
{


    int nRet = -1;         // статус выполнения


    if (CamIndex < 0 || CamIndex > stDeviceList1.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", Input Device Num error!" << endl;
        return nRet;
    }



    // Start grab image
    // Начало сбора изображений

    nRet = MV_CC_StartGrabbing(handle1[CamIndex]);



    if (MV_OK != nRet)
    {
        printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
        return nRet;
    }


    return nRet;
}

cv::Mat cl_CamHikRobot::HRCam_GetImage(unsigned int CamIndex)
{


    int nRet = -1;         // статус выполнения
    unsigned int    nTestFrameSize = 0;


    if (CamIndex < 0 || CamIndex > stDeviceList1.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", Input Device Num error!" << endl;



        Img =  cv::Mat::zeros(480,960,CV_8UC3);
        double coef_size = 960/1920;
        cv::putText	(Img,
                    "Input Device Num error",
                    {int(coef_size * 50), Img.rows - int(coef_size * 150)},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    coef_size * 5.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    (int) coef_size * 20.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);



        return  Img;
    }


    memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));


    nRet = MV_CC_GetImageForBGR(handle1[CamIndex], pFrameBuf, nBufSize, &stInfo, 1000);


    unsigned short width = stInfo.nWidth;
    unsigned short  height = stInfo.nHeight;

    std::cout << "Image size:: " << width << "x" << height << std::endl;


    // Фильтрация на ошибки потом добавить

    if (nRet != 0)
    {
        std::cout << "error:: GetImageForRGB:" << setbase(16) << nRet << std::endl;



        // =====================================
        //  No Signal изображение
        // =====================================
        Img =  cv::Mat::zeros(height,width,CV_8UC3);
        double coef_size = width/1920;
        cv::putText	(Img,
                    "No Signal",
                    {int(coef_size * 50), Img.rows - int(coef_size * 150)},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    coef_size * 5.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    (int) coef_size * 20.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin


    }
    else
    {
        //... Обработка данных изображения
        //std::cout << "imagetype:" << setbase(16) << stInfo.enPixelType << endl;

        if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
        {

            cv::Mat(height, width, CV_8UC3, pFrameBuf).copyTo(Img); // возращаемое изображение

        }// if

    } // if



    return Img;

}

int cl_CamHikRobot::HRCam_StopGrabbing(unsigned int CamIndex)
{

    int nRet = -1;         // статус выполнения

    // Stop grab image
    nRet = MV_CC_StopGrabbing(handle1[CamIndex]);
    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
        return nRet;
    }


    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> Stop Grabbing is done, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++


    return nRet;

}

int cl_CamHikRobot::HRCam_DisconnecCam(unsigned int CamIndex)
{
    int nRet = -1;         // статус выполнения
    // Close device
    nRet = MV_CC_CloseDevice(handle1[CamIndex]);
    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("Clos Device fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> Clos Device is done, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++

    return nRet;

}




// ==============================
// !!! УДАЛИТЬ :: Все эти функции скорее всего не нужны ::
// ==============================
// ==============================
// Работа с несколькими камерами по vector<unsigned int> CamIndex_S
// ==============================

//vector<int> cl_CamHikRobot::HRCam_ConnectToCam_S(vector<unsigned int> CamIndex_S)
//{


//    vector<int> nRet;         // статус выполнения
//    nRet.resize(CamIndex_S.size());
//    fill(nRet.begin(), nRet.end(), -1);

//    // Open device
//    // Подключаем устройство
//    unsigned int nAccessMode = MV_ACCESS_Exclusive;
//    unsigned short nSwitchoverKey = 0;


//    for(size_t q = 0; q < CamIndex_S.size(); ++q)
//    {


//        if (CamIndex_S.at(q)  < 0 || CamIndex_S.at(q)  > stDeviceList1.nDeviceNum)
//        {
//            cout << "CamIndex = " << CamIndex_S.at(q)  << ", Input Device Num error!" << endl;
//            nRet[q] = -1;
//            continue;
//        }


//        nRet[q] = MV_CC_OpenDevice(handle1[CamIndex_S.at(q)], nAccessMode, nSwitchoverKey);
//        //             nRet[q] = MV_CC_OpenDevice(handle1[CamIndex_S.at(q)] );

//        if (MV_OK != nRet[q])
//        {
//            cout << "CamIndex = " << CamIndex_S.at(q) << "  ";
//            printf("Open Device fail! nRet [0x%x]\n", nRet[q]);

//        }

//        // +++++++++++++++++++++++++++++
//        std::cout << " \t >>>  Device is Open, CamIndex = " <<  CamIndex_S.at(q) << std::endl;
//        // +++++++++++++++++++++++++++++

//    } // for


//    return nRet;


//}

//vector<int> cl_CamHikRobot::HRCam_SetCamSettings_S(vector<unsigned int> CamIndex_S)
//{

//    vector<int> nRet;         // статус выполнения
//    nRet.resize(CamIndex_S.size());
//    fill(nRet.begin(), nRet.end(), -1);



//    for(size_t q = 0; q < CamIndex_S.size(); ++q)
//    {


//        if (CamIndex_S[q]  < 0 || CamIndex_S[q]   > stDeviceList1.nDeviceNum)
//        {
//            cout << "CamIndex = " << CamIndex_S[q]  << ", Input Device Num error!" << endl;
//            nRet[q] = -1;
//            continue;
//        }


//        // Detection network optimal package size (It only works for the GigE camera)
//        if (stDeviceList1.pDeviceInfo[CamIndex_S.at(q) ]->nTLayerType == MV_GIGE_DEVICE)
//        {
//            int nPacketSize = MV_CC_GetOptimalPacketSize(handle1[CamIndex_S.at(q) ]);


//            if (nPacketSize > 0)
//            {
//                 nRet[q] = MV_CC_SetIntValue(handle1[CamIndex_S.at(q) ],"GevSCPSPacketSize",nPacketSize);

//                // +++++++++++++++++++++++++++++
//                std::cout << " \t >>>  Set Int Value, CamIndex = " <<  CamIndex_S[q]  << std::endl;
//                // +++++++++++++++++++++++++++++

//                if(nRet[q] != MV_OK)
//                {
//                    cout << "CamIndex_S = " << CamIndex_S[q]  << "  ";
//                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet[q]);
//                }
//            }
//            else
//            {
//                cout << "CamIndex_S = " << CamIndex_S.at(q) << "  ";
//                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
//            }
//        }



//        // Set trigger mode as off
//         nRet[q] = MV_CC_SetEnumValue(handle1[CamIndex_S.at(q)], "TriggerMode", 0);


//        if (MV_OK !=  nRet[q])
//        {
//            cout << "CamIndex = " << CamIndex_S.at(q) << "  ";
//            printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet[q]);
//            continue;
//        }

//        // +++++++++++++++++++++++++++++
//        std::cout << " \t >>> Set Trigger Mode, CamIndex = " <<  CamIndex_S.at(q) << std::endl;
//        // +++++++++++++++++++++++++++++




//        //  unsigned char * pData = NULL;


//        //        // Get payload size
//        //        // Получить размер кадра данных
//        //        MVCC_INTVALUE stParam;
//        //        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
//        //        nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
//        //        if (MV_OK != nRet)
//        //        {
//        //            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
//        //            break;
//        //        }
//        //        unsigned int nPayloadSize = stParam.nCurValue;


//        //        // int nBufSize = stParam.nCurValue; // Размер данных одного кадра


//        //        // +++++++++++++++++++++++++++++
//        //        std::cout << " \t >>> 10 Get payload size" << std::endl;
//        //        // +++++++++++++++++++++++++++++


//        //        // Init image info
//        //        MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
//        //        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
//        //        pData = (unsigned char *)malloc(sizeof(unsigned char)* (nPayloadSize));
//        //        if (NULL == pData)
//        //        {
//        //            printf("Allocate memory failed.\n");
//        //            break;
//        //        }
//        //        memset(pData, 0, sizeof(pData));


//        //        // +++++++++++++++++++++++++++++
//        //        std::cout << " \t >>> 11 Init image info" << std::endl;
//        //        // +++++++++++++++++++++++++++++

//    } // for
//    return nRet;


//}

//vector<int> cl_CamHikRobot::HRCam_StartGrabbing_S(vector<unsigned int> CamIndex_S)
//{

//    vector<int> nRet;         // статус выполнения
//    nRet.resize(CamIndex_S.size());
//    fill(nRet.begin(), nRet.end(), -1);



//    for(size_t q = 0; q < CamIndex_S.size(); ++q)
//    {


//        if (CamIndex_S[q]  < 0 || CamIndex_S[q]   > stDeviceList1.nDeviceNum)
//        {
//            cout << "CamIndex = " << CamIndex_S[q]  << ", Input Device Num error!" << endl;
//            nRet[q] = -1;
//            continue;
//        }




//    // Start grab image
//    // Начало сбора изображений

//    nRet[q] = MV_CC_StartGrabbing(handle1[CamIndex_S.at(q)]);



//    if (MV_OK != nRet[q])
//    {
//        printf("Start Grabbing fail! nRet [0x%x]\n", nRet[q]);
//        continue;
//    }

//        } // for
//    return nRet;


//}

//vector<cv::Mat> cl_CamHikRobot::HRCam_GetImage_S(vector<unsigned int> CamIndex_S)
//{

//}

//vector<int> cl_CamHikRobot::HRCam_StopGrabbing_S(vector<unsigned int> CamIndex_S)
//{

//}

//vector<int> cl_CamHikRobot::HRCam_DisconnecCam_S(vector<unsigned int> CamIndex_S)
//{

//}







void cl_CamHikRobot::HRCam_DestroyHandle(unsigned int CamIndex)
{

    // Destroy handle
    // Уничтожить дескриптор и освободить ресурсы

    if (handle1[CamIndex])
    {
        MV_CC_DestroyHandle(handle1[CamIndex]);
        handle1[CamIndex] = NULL;
    }


    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> Destroy Handle is done, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++

    //    // Free memery
    if (pFrameBuf)
    {
        free(pFrameBuf);
        pFrameBuf = NULL;
    }

    //system("pause");


    //    // Free memery
    //    if (pData)
    //    {
    //        free(pData);
    //        pData = NULL;
    //    }

    //system("pause");


}

MV_CC_DEVICE_INFO_LIST cl_CamHikRobot::HRCam_getDeviceList()
{
    return stDeviceList1;
}








